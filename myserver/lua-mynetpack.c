#include "skynet_malloc.h"

#include "skynet_socket.h"

#include <lua.h>
#include <lauxlib.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
	void * pack;
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
		skynet_free(np->pack);
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

	np->pack = skynet_malloc(np->size + HEADBUF_SIZE);
	memcpy(np->pack, np->headbuf, HEADBUF_SIZE);
	memcpy(np->pack + HEADBUF_SIZE, np->buffer, np->size);

	lua_pushinteger(L, np->id);
	lua_pushlightuserdata(L, np->pack);
	lua_pushinteger(L, np->size + HEADBUF_SIZE);

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

// static int
// lsetcmd(lua_State *L) {
// 	uint8_t* buffer = lua_touserdata(L, 1);
// 	uint16_t cmd = lua_tointeger(L, 2);
// 	cmd = htons(cmd);
// 	mynetpack_write_header((char*)buffer, (char*)&cmd, sizeof(uint16_t), HEADPOS_CMD);
// 	return 0;
// }

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

#define MAX_CMD_COUNT 1000
#define MAX_FIELD_COUNT 10
static int8_t tblmsg_formats[MAX_CMD_COUNT][MAX_FIELD_COUNT];
static void 
init_tblmsg_formats() {
	int i;
	for (i=0; i<MAX_CMD_COUNT; ++i) {
		tblmsg_formats[i][0] = -1; // mark non-existed message
	}
}
#include <stdio.h>
static void
print_tblmsg_formats() {
	int i,j;
	for (i=0; i<MAX_CMD_COUNT; ++i) {
		if (tblmsg_formats[i][0] == -1) continue;
		printf("cmd %d: [", i);
		for (j=0; j<MAX_FIELD_COUNT; ++j) {
			if (tblmsg_formats[i][j] == 0) break;
			printf("%d ", tblmsg_formats[i][j]);
		}
		printf("\b]\n");
	}
}
/*
_M.tblmsg_format = {}
_M.tblmsg_format[_M.tblmsg_test1] = { 1, 2, 2, 2, 1 } -- 1 for int; 2 for str
*/
static int 
lloadtblmsgformats(lua_State* L) {
	init_tblmsg_formats();

	lua_pushnil(L); // for next to start, -1:nil -2:table
	while (lua_next(L, -2)) {
		// -1:value -2:key -3:table
		int cmd = lua_tointeger(L, -2);

		// -1 is the sub table
		size_t len = lua_rawlen(L, -1);
		size_t i;
		for (i = 0; i < len; ++i) {
			lua_pushnumber(L, i+1);
			lua_gettable(L,-2);
			tblmsg_formats[cmd][i] = lua_tointeger(L,-1);
			lua_pop(L, 1);
		}
		tblmsg_formats[cmd][i] = 0; // mark field end

		// pop the value(sub table)
		lua_pop(L, 1);
	}
	// no need to pop key, lua_next pop it for us, -1:table

	print_tblmsg_formats();
	return 0;
}
static bool 
read_b(char* outbuf, int len, const char* buffer_, int size_, int* readptr_) {
    if ((len + *readptr_) > size_) { return false; }
    memcpy(outbuf, buffer_ + *readptr_, len);
    *readptr_ += len;
    return true;
}
static char* 
read_bytes_b(int len, const char* buffer_, int size_, int* readptr_) {
	if ((len + *readptr_) > size_) return NULL;
	char* p = (char*)&buffer_[*readptr_];
	*readptr_ += len;
	return p;
}
static int 
read_int32(const char* buffer_, int size_, int* readptr_) {
	int32_t val = -1;
	read_b((char*)&val, sizeof(int32_t), buffer_, size_, readptr_);
	return ntohl(val);
}
static char* 
read_string(const char* buffer_, int size_, int* readptr_) {
	int32_t len = read_int32(buffer_, size_, readptr_);
	if (len <= 0) return NULL;
	return read_bytes_b(len, buffer_, size_, readptr_);
}
static int
ltotable(lua_State* L) { // cmd, tbl = totable(msg,size)
	int readptr_ = HEADBUF_SIZE;
	int i;
	// uint16_t cmd = lua_tointeger(L, 1);
	char* buffer = (char*)lua_tostring(L, 1);
	// char* buffer = lua_touserdata(L, 1);
	int size = lua_tointeger(L, 2);
	// assert(mynetpack_command(buffer) == cmd);
	uint16_t cmd = mynetpack_command(buffer);
	// assert(tblmsg_formats[cmd][0] != -1);
	if (tblmsg_formats[cmd][0] == -1) {
		lua_pushinteger(L, cmd);
		return 1;
	}
	assert(buffer[0] == 'G');
	assert(buffer[1] == 'P');
	assert(mynetpack_version(buffer) == c_default_version);
	assert(mynetpack_subversion(buffer) == c_default_subversion);
	assert(mynetpack_checkcode(buffer) == 0);
	assert(mynetpack_bodylen(buffer) + HEADBUF_SIZE == size);

	lua_pushinteger(L, cmd);

	lua_newtable(L); // new table on top
	for (i = 0; tblmsg_formats[cmd][i] != 0; ++i) {
		// 1 for int; 2 for str
		switch (tblmsg_formats[cmd][i]) {
		case 1:
			lua_pushnumber(L, i + 1);
			lua_pushinteger(L, read_int32(buffer, size, &readptr_));
			lua_settable(L, -3);
			break;
		case 2:
			lua_pushnumber(L, i + 1);
			lua_pushstring(L, read_string(buffer, size, &readptr_));
			lua_settable(L, -3);
			break;
		default:
			printf("error tblmsg field type: %d\n", tblmsg_formats[cmd][i]);
			assert(0);
		}
	}

	return 2;
}
static bool 
write_b(const char* inbuf, int len, char* buffer_, int* size_, int blen) {
    if((*size_ < 0) || ((len + *size_) > blen))
    { return false; }
    memcpy(buffer_+*size_, inbuf, len);
    *size_ += len;
    return true;
}
static bool 
write_zero_b(char* buffer_, int* size_, int blen) {
    if((*size_ + 1) > blen)
    { return false; }
    memset(buffer_+*size_, '\0', sizeof(char));
    ++*size_;
    return true;
}
static bool 
write_int32(int32_t val, char* buffer_, int* size_, int blen) {
	val = htonl(val);
	return write_b((char*)&val, sizeof(int32_t), buffer_, size_, blen);
}
static bool
write_string(const char* str, char* buffer_, int* size_, int blen) {
	int len = (int)strlen(str);
	write_int32(len + 1, buffer_, size_, blen);
	return write_b(str, len, buffer_, size_, blen) &&
			write_zero_b(buffer_, size_, blen);
}
static int 
ltopack(lua_State* L) { // pack, size = topack(cmd, tblmsg)
	const int maxlen = 10240;
	int size_ = HEADBUF_SIZE;
	char* pack = skynet_malloc(maxlen);
	memset(pack, 0, maxlen);

	int cmd = lua_tointeger(L, -2);
	cmd = htons(cmd);
	mynetpack_write_header((char*)pack, (char*)&cmd, sizeof(uint16_t), HEADPOS_CMD);

	size_t len = lua_rawlen(L, -1);
	size_t i;
	for (i = 0; i < len; ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -2);
		if (lua_isinteger(L, -1)) {
			write_int32((int)lua_tointeger(L, -1), pack, &size_, maxlen);
		} else if (lua_isstring(L, -1)) {
			// write_string(lua_tostring(L, -1), pack, &size_, maxlen);
			char* str = (char*)lua_tostring(L, -1);
			printf("[lua-mynetpack.c] [ltopack] str: %s(%d)\n", str, (int)strlen(str));
			write_string(str, pack, &size_, maxlen);
		} else {
			printf("err: dont support %s\n", lua_typename(L, lua_type(L, -1)));
			lua_pop(L, 1);
			return 0;
		}
		lua_pop(L, 1);
	}

	write_size((uint8_t*)pack, size_ - HEADBUF_SIZE);

	lua_pushlightuserdata(L, pack);
	lua_pushinteger(L, size_);
	return 2;
}
// static int 
// ldeletepack(lua_State* L) {
// 	char* pack = lua_touserdata(L, 1);
// 	skynet_free(pack);
// 	return 0;
// }

int
luaopen_netpack(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "pop", lpop },
		{ "pack", lpack },
		// { "setcmd", lsetcmd },
		{ "clear", lclear },
		{ "tostring", ltostring },
		{ "loadtblmsgformats", lloadtblmsgformats },
		{ "totable", ltotable }, 
		{ "topack", ltopack },
		// { "deletepack", ldeletepack },
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
