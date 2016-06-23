#include "skynet_malloc.h"

#include "skynet_socket.h"

#include <lua.h>
#include <lauxlib.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#define QUEUESIZE 1024
#define HASHSIZE 4096
#define SMALLSTRING 2048

#define TYPE_DATA 1
#define TYPE_MORE 2
#define TYPE_ERROR 3
#define TYPE_OPEN 4
#define TYPE_CLOSE 5
#define TYPE_WARNING 6

/* header: | flag_2 | cmd_2 | version_1 | subversion_1 | bodylen_2 | checkcode_1 | */
#define HEADBUF_SIZE 9

#define HEADPOS_CMD 2
#define HEADPOS_VERSION 4
#define HEADPOS_SUBVERSION 5
#define HEADPOS_BODYLEN 6
#define HEADPOS_CHECKCODE 8

enum { c_default_version = 1, c_default_subversion = 1 };

static inline void mynetpack_read_header(const char* pack, char* outbuf, int len, int pos) {
	if (pos > 0 || pos + len < HEADBUF_SIZE) {
		memcpy(outbuf, pack + pos, len);
	}
}
static inline void mynetpack_write_header(char* pack, const char* inbuf, int len, int pos) {
	if (pos > 0 || pos + len < HEADBUF_SIZE) {
		memcpy(pack + pos, inbuf, len);
	}
}	
static inline uint16_t mynetpack_command(const char* pack) {
	uint16_t cmd;
	mynetpack_read_header(pack, (char*)&cmd, sizeof(uint16_t), HEADPOS_CMD);
	return ntohs(cmd);
}
static inline uint8_t mynetpack_version(const char* pack) {
	uint8_t ver;
	mynetpack_read_header(pack, (char*)&ver, sizeof(uint8_t), HEADPOS_VERSION);
	return ver;
}
static inline uint8_t mynetpack_subversion(const char* pack) {
	uint8_t subver;
	mynetpack_read_header(pack, (char*)&subver, sizeof(uint8_t), HEADPOS_SUBVERSION);
	return subver;
}
static inline uint16_t mynetpack_bodylen(const char* pack) {
	uint16_t bodylen;
	mynetpack_read_header(pack, (char*)&bodylen, sizeof(uint16_t), HEADPOS_BODYLEN);
	return ntohs(bodylen);
}
static inline uint8_t mynetpack_checkcode(const char* pack) {
	uint8_t cc;
	mynetpack_read_header(pack, (char*)&cc, sizeof(uint8_t), HEADPOS_CHECKCODE);
	return cc;
}

struct netpack {
	int id;
	int size;
	void * buffer;
	char headbuf[HEADBUF_SIZE];
};

struct uncomplete {
	struct netpack pack;
	struct uncomplete * next;
	int read;
	// int header;
	int headread;
};

struct queue {
	int cap;
	int head;
	int tail;
	struct uncomplete * hash[HASHSIZE];
	struct netpack queue[QUEUESIZE];
};

static void
clear_list(struct uncomplete * uc) {
	while (uc) {
		skynet_free(uc->pack.buffer);
		void * tmp = uc;
		uc = uc->next;
		skynet_free(tmp);
	}
}

static int
lclear(lua_State *L) {
	struct queue * q = lua_touserdata(L, 1);
	if (q == NULL) {
		return 0;
	}
	int i;
	for (i=0;i<HASHSIZE;i++) {
		clear_list(q->hash[i]);
		q->hash[i] = NULL;
	}
	if (q->head > q->tail) {
		q->tail += q->cap;
	}
	for (i=q->head;i<q->tail;i++) {
		struct netpack *np = &q->queue[i % q->cap];
		skynet_free(np->buffer);
	}
	q->head = q->tail = 0;

	return 0;
}

static inline int
hash_fd(int fd) {
	int a = fd >> 24;
	int b = fd >> 12;
	int c = fd;
	return (int)(((uint32_t)(a + b + c)) % HASHSIZE);
}

static struct uncomplete *
find_uncomplete(struct queue *q, int fd) {
	if (q == NULL)
		return NULL;
	int h = hash_fd(fd);
	struct uncomplete * uc = q->hash[h];
	if (uc == NULL)
		return NULL;
	if (uc->pack.id == fd) {
		q->hash[h] = uc->next;
		return uc;
	}
	struct uncomplete * last = uc;
	while (last->next) {
		uc = last->next;
		if (uc->pack.id == fd) {
			last->next = uc->next;
			return uc;
		}
		last = uc;
	}
	return NULL;
}

static struct queue *
get_queue(lua_State *L) {
	struct queue *q = lua_touserdata(L,1);
	if (q == NULL) {
		q = lua_newuserdata(L, sizeof(struct queue));
		q->cap = QUEUESIZE;
		q->head = 0;
		q->tail = 0;
		int i;
		for (i=0;i<HASHSIZE;i++) {
			q->hash[i] = NULL;
		}
		lua_replace(L, 1);
	}
	return q;
}

static void
expand_queue(lua_State *L, struct queue *q) {
	struct queue *nq = lua_newuserdata(L, sizeof(struct queue) + q->cap * sizeof(struct netpack));
	nq->cap = q->cap + QUEUESIZE;
	nq->head = 0;
	nq->tail = q->cap;
	memcpy(nq->hash, q->hash, sizeof(nq->hash));
	memset(q->hash, 0, sizeof(q->hash));
	int i;
	for (i=0;i<q->cap;i++) {
		int idx = (q->head + i) % q->cap;
		nq->queue[i] = q->queue[idx];
	}
	q->head = q->tail = 0;
	lua_replace(L,1);
}

static void
push_data(lua_State *L, int fd, void *buffer, int size, int clone, void* headbuf) {
	if (clone) {
		void * tmp = skynet_malloc(size);
		memcpy(tmp, buffer, size);
		buffer = tmp;
	}
	struct queue *q = get_queue(L);
	struct netpack *np = &q->queue[q->tail];
	if (++q->tail >= q->cap)
		q->tail -= q->cap;
	np->id = fd;
	np->buffer = buffer;
	np->size = size;
	memcpy(np->headbuf, headbuf, HEADBUF_SIZE);
	if (q->head == q->tail) {
		expand_queue(L, q);
	}
}

static struct uncomplete *
save_uncomplete(lua_State *L, int fd) {
	struct queue *q = get_queue(L);
	int h = hash_fd(fd);
	struct uncomplete * uc = skynet_malloc(sizeof(struct uncomplete));
	memset(uc, 0, sizeof(*uc));
	uc->next = q->hash[h];
	uc->pack.id = fd;
	q->hash[h] = uc;

	return uc;
}

static inline int
read_size(uint8_t * buffer) {
	int bodylen = mynetpack_bodylen((const char*)buffer);
	return bodylen;
}

static void
push_more(lua_State *L, int fd, uint8_t *buffer, int size) {
	if (size < HEADBUF_SIZE) {
		struct uncomplete* uc = save_uncomplete(L, fd);
		uc->read = -1;
		memcpy(uc->pack.headbuf, buffer, size);
		uc->headread = size;
		return;
	}
	int pack_size = read_size(buffer);
	buffer += HEADBUF_SIZE;
	size -= HEADBUF_SIZE;

	if (size < pack_size) {
		struct uncomplete * uc = save_uncomplete(L, fd);
		uc->read = size;
		uc->pack.size = pack_size;
		uc->pack.buffer = skynet_malloc(pack_size);
		memcpy(uc->pack.buffer, buffer, size);

		memcpy(uc->pack.headbuf, buffer - HEADBUF_SIZE, HEADBUF_SIZE);
		uc->headread = HEADBUF_SIZE;

		return;
	}
	push_data(L, fd, buffer, pack_size, 1, buffer - HEADBUF_SIZE);

	buffer += pack_size;
	size -= pack_size;
	if (size > 0) {
		push_more(L, fd, buffer, size);
	}
}

static void
close_uncomplete(lua_State *L, int fd) {
	struct queue *q = lua_touserdata(L,1);
	struct uncomplete * uc = find_uncomplete(q, fd);
	if (uc) {
		skynet_free(uc->pack.buffer);
		skynet_free(uc);
	}
}

static int
filter_data_(lua_State *L, int fd, uint8_t * buffer, int size) {
	struct queue *q = lua_touserdata(L,1);
	struct uncomplete * uc = find_uncomplete(q, fd);
	if (uc) {
		// fill uncomplete
		if (uc->read < 0) {
			assert(uc->read == -1);

			if (uc->headread + size < HEADBUF_SIZE) {
				memcpy(uc->pack.headbuf + uc->headread, buffer, size);
				uc->headread += size;
				return 1;
			}

			int headleft = HEADBUF_SIZE - uc->headread;
			memcpy(uc->pack.headbuf + uc->headread, buffer, headleft);
			uc->headread += headleft;
			assert(uc->headread == HEADBUF_SIZE);

			size -= headleft;
			buffer += headleft;

			// read size
			int pack_size = mynetpack_bodylen(uc->pack.headbuf);
			uc->pack.size = pack_size;
			uc->pack.buffer = skynet_malloc(pack_size);
			uc->read = 0;
		}
		int need = uc->pack.size - uc->read;
		if (size < need) {
			memcpy(uc->pack.buffer + uc->read, buffer, size);
			uc->read += size;
			int h = hash_fd(fd);
			uc->next = q->hash[h];
			q->hash[h] = uc;
			return 1;
		}
		memcpy(uc->pack.buffer + uc->read, buffer, need);
		buffer += need;
		size -= need;
		if (size == 0) {
			lua_pushvalue(L, lua_upvalueindex(TYPE_DATA));
			lua_pushinteger(L, fd);
			
			void* result = skynet_malloc(uc->pack.size + HEADBUF_SIZE);
			memcpy(result, uc->pack.headbuf, HEADBUF_SIZE);
			memcpy(result, uc->pack.buffer, uc->pack.size);
			
			lua_pushlightuserdata(L, result);
			lua_pushinteger(L, uc->pack.size + HEADBUF_SIZE);

			skynet_free(uc->pack.buffer);
			skynet_free(uc);
			return 5;
		}
		// more data
		push_data(L, fd, uc->pack.buffer, uc->pack.size, 0, uc->pack.headbuf);
		skynet_free(uc);
		push_more(L, fd, buffer, size);
		lua_pushvalue(L, lua_upvalueindex(TYPE_MORE));
		return 2;
	} else {
		if (size < HEADBUF_SIZE) {
			struct uncomplete* uc = save_uncomplete(L, fd);
			uc->read = -1;
			memcpy(uc->pack.headbuf, buffer, size);
			uc->headread = size;
			return 1;
		}
		int pack_size = read_size(buffer);
		buffer += HEADBUF_SIZE;
		size -= HEADBUF_SIZE;

		if (size < pack_size) {
			struct uncomplete * uc = save_uncomplete(L, fd);
			uc->read = size;
			uc->pack.size = pack_size;
			uc->pack.buffer = skynet_malloc(pack_size);
			memcpy(uc->pack.buffer, buffer, size);

			memcpy(uc->pack.headbuf, buffer - HEADBUF_SIZE, HEADBUF_SIZE);
			uc->headread = HEADBUF_SIZE;

			return 1;
		}
		if (size == pack_size) {
			// just one package
			lua_pushvalue(L, lua_upvalueindex(TYPE_DATA));
			lua_pushinteger(L, fd);
			void * result = skynet_malloc(HEADBUF_SIZE + pack_size);
			memcpy(result, buffer - HEADBUF_SIZE, HEADBUF_SIZE + size);
			lua_pushlightuserdata(L, result);
			lua_pushinteger(L, HEADBUF_SIZE + size);
			return 5;
		}
		// more data
		push_data(L, fd, buffer, pack_size, 1, buffer - HEADBUF_SIZE); 
		buffer += pack_size;
		size -= pack_size;
		push_more(L, fd, buffer, size);
		lua_pushvalue(L, lua_upvalueindex(TYPE_MORE));
		return 2;
	}
}

static inline int
filter_data(lua_State *L, int fd, uint8_t * buffer, int size) {
	int ret = filter_data_(L, fd, buffer, size);
	// buffer is the data of socket message, it malloc at socket_server.c : function forward_message .
	// it should be free before return,
	skynet_free(buffer);
	return ret;
}

static void
pushstring(lua_State *L, const char * msg, int size) {
	if (msg) {
		lua_pushlstring(L, msg, size);
	} else {
		lua_pushliteral(L, "");
	}
}

/*
	userdata queue
	lightuserdata msg
	integer size
	return
		userdata queue
		integer type
		integer fd
		string msg | lightuserdata/integer
 */
static int
lfilter(lua_State *L) {
	struct skynet_socket_message *message = lua_touserdata(L,2);
	int size = luaL_checkinteger(L,3);
	char * buffer = message->buffer;
	if (buffer == NULL) {
		buffer = (char *)(message+1);
		size -= sizeof(*message);
	} else {
		size = -1;
	}

	lua_settop(L, 1);

	switch(message->type) {
	case SKYNET_SOCKET_TYPE_DATA:
		// ignore listen id (message->id)
		assert(size == -1);	// never padding string
		return filter_data(L, message->id, (uint8_t *)buffer, message->ud);
	case SKYNET_SOCKET_TYPE_CONNECT:
		// ignore listen fd connect
		return 1;
	case SKYNET_SOCKET_TYPE_CLOSE:
		// no more data in fd (message->id)
		close_uncomplete(L, message->id);
		lua_pushvalue(L, lua_upvalueindex(TYPE_CLOSE));
		lua_pushinteger(L, message->id);
		return 3;
	case SKYNET_SOCKET_TYPE_ACCEPT:
		lua_pushvalue(L, lua_upvalueindex(TYPE_OPEN));
		// ignore listen id (message->id);
		lua_pushinteger(L, message->ud);
		pushstring(L, buffer, size);
		return 4;
	case SKYNET_SOCKET_TYPE_ERROR:
		// no more data in fd (message->id)
		close_uncomplete(L, message->id);
		lua_pushvalue(L, lua_upvalueindex(TYPE_ERROR));
		lua_pushinteger(L, message->id);
		pushstring(L, buffer, size);
		return 4;
	case SKYNET_SOCKET_TYPE_WARNING:
		lua_pushvalue(L, lua_upvalueindex(TYPE_WARNING));
		lua_pushinteger(L, message->id);
		lua_pushinteger(L, message->ud);
		return 4;
	default:
		// never get here
		return 1;
	}
}

/*
	userdata queue
	return
		integer fd
		lightuserdata msg
		integer size
 */
static int
lpop(lua_State *L) {
	struct queue * q = lua_touserdata(L, 1);
	if (q == NULL || q->head == q->tail)
		return 0;
	struct netpack *np = &q->queue[q->head];
	if (++q->head >= q->cap) {
		q->head = 0;
	}
	lua_pushinteger(L, np->id);
	lua_pushlightuserdata(L, np->buffer);
	lua_pushinteger(L, np->size);

	return 3;
}

/*
	string msg | lightuserdata/integer

	lightuserdata/integer
 */

static const char *
tolstring(lua_State *L, size_t *sz, int index) {
	const char * ptr;
	if (lua_isuserdata(L,index)) {
		ptr = (const char *)lua_touserdata(L,index);
		*sz = (size_t)luaL_checkinteger(L, index+1);
	} else {
		ptr = luaL_checklstring(L, index, sz);
	}
	return ptr;
}

static inline void
write_size(uint8_t * buffer, int len) {
	buffer[0] = 'G';
	buffer[1] = 'P';
	
	uint16_t bodylen = htons(len);
	mynetpack_write_header((char*)buffer, (char*)&bodylen, sizeof(uint16_t), HEADPOS_BODYLEN);

	uint8_t ver = c_default_version;
	uint8_t sver = c_default_subversion;
	uint8_t cc = 0;
	mynetpack_write_header((char*)buffer, (char*)&ver, sizeof(uint8_t), HEADPOS_VERSION);
	mynetpack_write_header((char*)buffer, (char*)&sver, sizeof(uint8_t), HEADPOS_SUBVERSION);
	mynetpack_write_header((char*)buffer, (char*)&cc, sizeof(uint8_t), HEADPOS_CHECKCODE);
}

static int
lsetcmd(lua_State *L) {
	// TODO

	return 0;
}

static int
lpack(lua_State *L) {
	size_t len;
	const char * ptr = tolstring(L, &len, 1);
	if (len >= 0x10000) {
		return luaL_error(L, "Invalid size (too long) of data : %d", (int)len);
	}

	uint8_t * buffer = skynet_malloc(len + HEADBUF_SIZE);
	write_size(buffer, len);
	memcpy(buffer + HEADBUF_SIZE, ptr, len);

	lua_pushlightuserdata(L, buffer);
	lua_pushinteger(L, len + HEADBUF_SIZE);

	return 2;
}

static int
ltostring(lua_State *L) {
	void * ptr = lua_touserdata(L, 1);
	int size = luaL_checkinteger(L, 2);
	if (ptr == NULL) {
		lua_pushliteral(L, "");
	} else {
		lua_pushlstring(L, (const char *)ptr, size);
		skynet_free(ptr);
	}
	return 1;
}

int
luaopen_netpack(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "pop", lpop },
		{ "pack", lpack },
		{ "setcmd", lsetcmd },
		{ "clear", lclear },
		{ "tostring", ltostring },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);

	// the order is same with macros : TYPE_* (defined top)
	lua_pushliteral(L, "data");
	lua_pushliteral(L, "more");
	lua_pushliteral(L, "error");
	lua_pushliteral(L, "open");
	lua_pushliteral(L, "close");
	lua_pushliteral(L, "warning");

	lua_pushcclosure(L, lfilter, 6);
	lua_setfield(L, -2, "filter");

	return 1;
}
