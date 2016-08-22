#include "skynet_malloc.h"
#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#if defined(_WIN32) || defined(__APPLE__) 
	#define IS_LITTLE_ENDIAN 
#else
    #include <endian.h> 
	#if (__BYTE_ORDER == __BIG_ENDIAN)
		#define IS_BIG_ENDIAN
	#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
		#define IS_LITTLE_ENDIAN
	#else
		#error endianness unknown
	#endif
#endif /// defined(_WIN32) || defined(__APPLE__)

#ifdef IS_BIG_ENDIAN
	static inline int8_t endianopi8(int8_t v) { return v; }
	static inline uint8_t endianopu8(uint8_t v) { return v; }
	static inline int16_t endianopi16(int16_t v) { return v; }
	static inline uint16_t endianopu16(uint16_t v) { return v; }
	static inline int32_t endianopi32(int32_t v) { return v; }
	static inline uint32_t endianopu32(uint32_t v) { return v; }
	static inline int64_t endianopi64(int64_t v) { return v; }
	static inline uint64_t endianopu64(uint64_t v) { return v; }
#else /* IS_LITTLE_ENDIAN */
	// static inline int8_t endianopi8(int8_t v) { return v; }
	static inline uint8_t endianopu8(uint8_t v) { return v; }
	static inline int16_t endianopi16(int16_t v) {
		int16_t result;
		((unsigned char*)&result)[0] = ((const unsigned char*)&v)[1];
		((unsigned char*)&result)[1] = ((const unsigned char*)&v)[0];
		return result;
	}
	static inline uint16_t endianopu16(uint16_t v) { 
		uint16_t result;
		((unsigned char*)&result)[0] = ((const unsigned char*)&v)[1];
		((unsigned char*)&result)[1] = ((const unsigned char*)&v)[0];
		return result;
	}
	static inline int32_t endianopi32(int32_t v) { 
		int32_t result;
		((unsigned char*)&result)[0] = ((const unsigned char*)&v)[3];
		((unsigned char*)&result)[1] = ((const unsigned char*)&v)[2];
		((unsigned char*)&result)[2] = ((const unsigned char*)&v)[1];
		((unsigned char*)&result)[3] = ((const unsigned char*)&v)[0];
		return result;
	}
	static inline uint32_t endianopu32(uint32_t v) { 
		uint32_t result;
		((unsigned char*)&result)[0] = ((const unsigned char*)&v)[3];
		((unsigned char*)&result)[1] = ((const unsigned char*)&v)[2];
		((unsigned char*)&result)[2] = ((const unsigned char*)&v)[1];
		((unsigned char*)&result)[3] = ((const unsigned char*)&v)[0];
		return result;
	}
	// static inline int64_t endianopi64(int64_t v) { 
	// 	int64_t result;
	// 	((unsigned char*)&result)[0] = ((const unsigned char*)&v)[7];
	// 	((unsigned char*)&result)[1] = ((const unsigned char*)&v)[6];
	// 	((unsigned char*)&result)[2] = ((const unsigned char*)&v)[5];
	// 	((unsigned char*)&result)[3] = ((const unsigned char*)&v)[4];
	// 	((unsigned char*)&result)[4] = ((const unsigned char*)&v)[3];
	// 	((unsigned char*)&result)[5] = ((const unsigned char*)&v)[2];
	// 	((unsigned char*)&result)[6] = ((const unsigned char*)&v)[1];
	// 	((unsigned char*)&result)[7] = ((const unsigned char*)&v)[0];
	// 	return result;
	// }
	// static inline uint64_t endianopu64(uint64_t v) { 
	// 	uint64_t result;
	// 	((unsigned char*)&result)[0] = ((const unsigned char*)&v)[7];
	// 	((unsigned char*)&result)[1] = ((const unsigned char*)&v)[6];
	// 	((unsigned char*)&result)[2] = ((const unsigned char*)&v)[5];
	// 	((unsigned char*)&result)[3] = ((const unsigned char*)&v)[4];
	// 	((unsigned char*)&result)[4] = ((const unsigned char*)&v)[3];
	// 	((unsigned char*)&result)[5] = ((const unsigned char*)&v)[2];
	// 	((unsigned char*)&result)[6] = ((const unsigned char*)&v)[1];
	// 	((unsigned char*)&result)[7] = ((const unsigned char*)&v)[0];
	// 	return result;
	// }
#endif /// IS_BIG_ENDIAN

#define BOXMSGHEADSZ 16
#define BOXMSGBUFCAP 8192

struct boxmsg {
	int size;
	int readpos;
	/* ver_1 | headsz_1 | cmd_2 | flag_4 | sid_4 | bodylen_4 | body_bodylen */
	char buf[BOXMSGBUFCAP];
};

struct parseinfo {
	struct parseinfo* next;
	int id;

	int state;
	size_t packpos;
	size_t bodylen;
	struct boxmsg msg;
};

struct boxmsg_chain {
	struct boxmsg_chain* next;
	struct boxmsg msg;
};

#define HASHSIZE 4096

struct parseinfo_table {
	struct parseinfo* hash[HASHSIZE];
};

enum {
	box_version1 = 1,
	box_version2,
	box_version3,
	box_versioninvalid
};

enum {
	parse_head = 0,
	parse_body,
	parse_done,
	parse_error
};

static void
boxmsg_reset(struct boxmsg* msg) {
	msg->size = BOXMSGHEADSZ;
	msg->readpos = BOXMSGHEADSZ;
}
static void 
boxmsg_read_header(struct boxmsg* msg, char* outbuf, int len, int pos) {
	if (pos >= 0 && pos + len <= BOXMSGHEADSZ)
		memcpy(outbuf, msg->buf + pos, len);
}
static uint8_t
boxmsg_version(struct boxmsg* msg) {
	uint8_t v;
	boxmsg_read_header(msg, (char*)&v, sizeof(uint8_t), 0);
	return endianopu8(v);
}
static uint8_t
boxmsg_headsz(struct boxmsg* msg) {
	uint8_t hs;
	boxmsg_read_header(msg, (char*)&hs, sizeof(uint8_t), 1);
	return endianopu8(hs);
}
static uint16_t
boxmsg_cmd(struct boxmsg* msg) {
	uint16_t cmd;
	boxmsg_read_header(msg, (char*)&cmd, sizeof(uint16_t), 2);
	return endianopu16(cmd);
}
static uint32_t
boxmsg_flag(struct boxmsg* msg) {
	uint32_t flag;
	boxmsg_read_header(msg, (char*)&flag, sizeof(uint32_t), 4);
	return endianopu32(flag);
}
static uint32_t
boxmsg_sid(struct boxmsg* msg) {
	uint32_t sid;
	boxmsg_read_header(msg, (char*)&sid, sizeof(uint32_t), 8);
	return endianopu32(sid);
}
static uint32_t
boxmsg_bodylen(struct boxmsg* msg) {
	uint32_t bodylen;
	boxmsg_read_header(msg, (char*)&bodylen, sizeof(uint32_t), 12);
	return endianopu32(bodylen);
}
static int
boxmsg_read(struct boxmsg* msg, char* outbuf, int len) {
	if ((len + msg->readpos) > msg->size || (len + msg->readpos) > BOXMSGBUFCAP)
		return 0; // false
	memcpy(outbuf, msg->buf + msg->readpos, len);
	msg->readpos += len;
	return 1; // true
}
static char*
boxmsg_readbytes(struct boxmsg* msg, int len) {
	if ((len + msg->readpos) > msg->size) return NULL;
	char* p = &(msg->buf[msg->readpos]);
	msg->readpos += len;
	return p;
}
static void
boxmsg_readundo(struct boxmsg* msg, int len) {
	msg->readpos -= len;
}
static void
boxmsg_writehead(struct boxmsg* msg, const char* inbuf, int len, int pos) {
	if (pos >= 0 && pos + len <= BOXMSGHEADSZ) 
		memcpy(msg->buf + pos, inbuf, len);
}
static void
boxmsg_begin(struct boxmsg* msg, uint16_t cmd) {
	boxmsg_reset(msg);
	cmd = endianopu16(cmd);
	uint8_t version = box_version1;
	uint8_t headsz = BOXMSGHEADSZ;
	uint32_t flag = 0;
	uint32_t sid = 0;
	uint32_t bodylen = 0;
	boxmsg_writehead(msg, (char*)&version, sizeof(version), 0);
	boxmsg_writehead(msg, (char*)&headsz, sizeof(headsz), 1);
	boxmsg_writehead(msg, (char*)&cmd, sizeof(cmd), 2);
	boxmsg_writehead(msg, (char*)&flag, sizeof(flag), 4);
	boxmsg_writehead(msg, (char*)&sid, sizeof(sid), 8);
	boxmsg_writehead(msg, (char*)&bodylen, sizeof(bodylen), 12);
}
static void 
boxmsg_end(struct boxmsg* msg) {
 	uint32_t bodylen = msg->size - BOXMSGHEADSZ;
 	bodylen = endianopu32(bodylen);
 	boxmsg_writehead(msg, (char*)&bodylen, sizeof(uint32_t), 12);
}
// static int 
// boxmsg_readdel(struct boxmsg* msg, char* outbuf, int len) {
// 	if (!boxmsg_read(msg, outbuf, len)) return 0; // false
// 	memmove(msg->buf + msg->readpos - len, msg->buf + msg->readpos, msg->size - msg->readpos);
// 	msg->readpos -= len;
// 	msg->size -= len;
// 	boxmsg_end(msg);
// 	return 1; // true
// }
// static uint8_t
// boxmsg_readu8(struct boxmsg* msg) {
// 	uint8_t v;
// 	boxmsg_read(msg, (char*)&v, sizeof(v));
// 	return endianopu8(v);
// }
// static int8_t
// boxmsg_readi8(struct boxmsg* msg) {
// 	int8_t v;
// 	boxmsg_read(msg, (char*)&v, sizeof(v));
// 	return endianopi8(v);
// }
static uint16_t
boxmsg_readu16(struct boxmsg* msg) {
	uint16_t v;
	boxmsg_read(msg, (char*)&v, sizeof(v));
	return endianopu16(v);
}
// static int16_t
// boxmsg_readi16(struct boxmsg* msg) {
// 	int16_t v;
// 	boxmsg_read(msg, (char*)&v, sizeof(v));
// 	return endianopi16(v);
// }
static uint32_t
boxmsg_readu32(struct boxmsg* msg) {
	uint32_t v;
	boxmsg_read(msg, (char*)&v, sizeof(v));
	return endianopu32(v);
}
static int32_t
boxmsg_readi32(struct boxmsg* msg) {
	int32_t v;
	boxmsg_read(msg, (char*)&v, sizeof(v));
	return endianopi32(v);
}
// static uint64_t
// boxmsg_readu64(struct boxmsg* msg) {
// 	uint64_t v;
// 	boxmsg_read(msg, (char*)&v, sizeof(v));
// 	return endianopu64(v);
// }
// static int64_t
// boxmsg_readi64(struct boxmsg* msg) {
// 	int64_t v;
// 	boxmsg_read(msg, (char*)&v, sizeof(v));
// 	return endianopi64(v);
// }
static char*
boxmsg_readcstring(struct boxmsg* msg) {
	int len = boxmsg_readi32(msg);
	if (len <= 0 || len >= BOXMSGBUFCAP) return NULL;
	return boxmsg_readbytes(msg, len);
}
static int
boxmsg_readbinary(struct boxmsg* msg, char* outbuf, int maxlen) {
	int len = boxmsg_readi32(msg);
	if (len <= 0) return -1;
	if (len > maxlen) {
		boxmsg_readundo(msg, sizeof(int32_t));
		return -1;
	}
	if (boxmsg_read(msg, outbuf, len)) return len;
	return 0;
}
static int 
boxmsg_write(struct boxmsg* msg, const char* inbuf, int len) {
	if (msg->size < 0 || (len + msg->size) > BOXMSGBUFCAP) return 0; // false
	memcpy(msg->buf + msg->size, inbuf, len);
	msg->size += len;
	return 1; // true
}
static int 
boxmsg_writeu16(struct boxmsg* msg, uint16_t v) {
	v = endianopu16(v);
	return boxmsg_write(msg, (char*)&v, sizeof(v));
}
static int
boxmsg_writei16(struct boxmsg* msg, int16_t v) {
	v = endianopi16(v);
	return boxmsg_write(msg, (char*)&v, sizeof(v));
}
static int
boxmsg_writeu32(struct boxmsg* msg, uint32_t v) {
	v = endianopu32(v);
	return boxmsg_write(msg, (char*)&v, sizeof(v));
}
static int
boxmsg_writei32(struct boxmsg* msg, int32_t v) {
	v = endianopi32(v);
	return boxmsg_write(msg, (char*)&v, sizeof(v));
}
static int
boxmsg_writezero(struct boxmsg* msg) {
	if ((msg->size + 1) > BOXMSGBUFCAP) return 0; // false
	memset(msg->buf + msg->size, '\0', sizeof(char));
	msg->size += 1;
	return 1; // true
}
static int
boxmsg_writecstring(struct boxmsg* msg, const char* cstr) {
	int len = (int32_t)strlen(cstr);
	boxmsg_writei32(msg, len + 1);
	return boxmsg_write(msg, cstr, len) && boxmsg_writezero(msg);
}
static int 
boxmsg_writebinary(struct boxmsg* msg, const char* inbuf, int len) {
	boxmsg_writei32(msg, len);
	return boxmsg_write(msg, inbuf, len);
}

static inline int
hash_id(int id) {
	int a = id >> 24;
	int b = id >> 12;
	int c = id;
	return (int)(((uint32_t)(a + b + c)) % HASHSIZE);
}

static struct parseinfo_table*
get_parseinfo_table(lua_State* L) {
	struct parseinfo_table* t = lua_touserdata(L, 1);
	assert(t);
	// if (t == NULL) {
	// 	t = lua_newuserdata(L, sizeof(struct parseinfo_table));
	// 	int i;
	// 	for (i=0; i<HASHSIZE; ++i) {
	// 		t->hash[i] = NULL;
	// 	}
	// 	lua_replace(L, 1);
	// }
	return t;
}

static struct parseinfo*
find_parseinfo(struct parseinfo_table* t, int id) {
	if (t == NULL)
		return NULL;
	int h = hash_id(id);
	struct parseinfo* msg = t->hash[h];
	if (msg == NULL)
		return NULL;
	if (msg->id == id) {
		t->hash[h] = msg->next;
		return msg;
	}
	struct parseinfo* last = msg;
	while (last->next) {
		msg = last->next;
		if (msg->id == id) {
			last->next = msg->next;
			return msg;
		}
		last = msg;
	}
	return NULL;
}

static void
parseinfo_reset(struct parseinfo* p) {
	p->state = parse_head;
	p->packpos = 0;
	p->bodylen = 0;
	boxmsg_reset(&(p->msg));
}

/* boxproto.init(table, id) */
static int
linit(lua_State* L) {
	struct parseinfo_table* t = get_parseinfo_table(L);
	int id = luaL_checkinteger(L, 2);
	struct parseinfo* p = (struct parseinfo*)skynet_malloc(sizeof(struct parseinfo));
	parseinfo_reset(p);
	int h = hash_id(id);
	p->next = t->hash[h];
	p->id = id;
	t->hash[h] = p;
	return 0;
}

/* boxproto.fini(table, id) */
static int
lfini(lua_State* L) {
	struct parseinfo_table* t = get_parseinfo_table(L);
	int id = luaL_checkinteger(L, 2);
	struct parseinfo* p = find_parseinfo(t, id);
	if (p) {
		skynet_free(p);
	}
	return 0;
}

static int 
read_head(struct parseinfo* p, const char* data, const size_t length, size_t* pndx) {
	while (p->packpos < BOXMSGHEADSZ && *pndx < length) {
		p->msg.buf[p->packpos++] = data[(*pndx)++];
	}
	if (p->packpos < BOXMSGHEADSZ) return 0; // not enough head yet
	return 1; // head enough
}

static int
parse_header(struct parseinfo* p) {
	uint8_t version = boxmsg_version(&(p->msg));
	uint8_t headsz = boxmsg_headsz(&(p->msg));
	if (headsz != BOXMSGHEADSZ || version < box_version1 || version >= box_versioninvalid) {
		// invalid headsz or invlaid version
		return -2;
	}
	p->bodylen = boxmsg_bodylen(&(p->msg));
	if (p->bodylen <= (BOXMSGBUFCAP - BOXMSGHEADSZ)) return 0; // valid
	// invalid bodylen
	return -4;
}

static int
read_body(struct parseinfo* p, const char* data, const size_t length, size_t* pndx) {
	size_t needlen = (p->bodylen + BOXMSGHEADSZ) - p->packpos;
	if (needlen <= 0) return 1; // done
	size_t buflen = length - *pndx;
	if (buflen <= 0) return 0; // not enough data
	size_t cplen = buflen < needlen ? buflen : needlen;
	if (!boxmsg_write(&(p->msg), data + *pndx, (int)cplen)) return 0; // write error
	p->packpos += cplen;
	(*pndx) += cplen;
	if (p->packpos < (p->bodylen + BOXMSGHEADSZ)) return 0; // not enough data
	return 1; // done
}

#include <stdio.h>
/* chain, error = boxproto.parse(table, id, str) */
static int
lparse(lua_State* L) {
	struct parseinfo_table* t = get_parseinfo_table(L);
	int id = luaL_checkinteger(L, 2);
	size_t length = 0;
	const char* data = luaL_checklstring(L, 3, &length);
	struct parseinfo* p = find_parseinfo(t, id);
	printf("c: id %d\n", id);
	assert(p);

	int msgcnt = 0;
	struct boxmsg_chain* chain = NULL;
	struct boxmsg_chain* last = NULL;
	size_t ndx = 0;
	while (ndx < length && p->state != parse_error) {
		switch (p->state) {
			case parse_head:
				if (!read_head(p, data, length, &ndx)) {
					break;
				}
				if (parse_header(p) != 0) {
					p->state = parse_error;
					break;
				}
				else {
					p->state = parse_body;
					// dont break, continue parsing body
				}
			case parse_body:
				if (read_body(p, data, length, &ndx)) {
					p->state = parse_done;
				}
				break;
			default:
				// log
				lua_pushnil(L);
				lua_pushinteger(L, -1);
				return 2;
		}
		if (p->state == parse_error) parseinfo_reset(p);
		if (p->state == parse_done) {
			if (chain == NULL) {
				chain = (struct boxmsg_chain*)skynet_malloc(sizeof(*chain));
				memcpy((char*)&(chain->msg), (char*)&(p->msg), sizeof(p->msg));
				chain->next = NULL;
				last = chain;
			} else {
				last->next = (struct boxmsg_chain*)skynet_malloc(sizeof(*chain));
				memcpy((char*)&(last->next->msg), (char*)&(p->msg), sizeof(p->msg));
				last->next->next = NULL;
				last = last->next;
			}
			msgcnt += 1;
			parseinfo_reset(p);
		}
	}
	if (chain == NULL) {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
		return 2;
	} else {
		lua_pushlightuserdata(L, chain);
		lua_pushinteger(L, msgcnt);
		return 2;
	}
}

/* msg, left = boxproto.takefront(chain) */
static int
ltakefront(lua_State* L) {
	struct boxmsg_chain* chain = lua_touserdata(L, 1);
	if (chain == NULL) {
		lua_pushnil(L);
		return 1;
	}
	struct boxmsg_chain* next = chain->next;
	chain->next = NULL;
	lua_pushlightuserdata(L, chain);
	lua_pushlightuserdata(L, next);
	return 2;
}

/* boxproto.freechainnode(msgnode) */
static int
lfreechainnode(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	if (chainnode == NULL) return 0;
	skynet_free(chainnode);
	return 0;
}

/* pt = boxproto.createparseinfotable() */
static int 
lcreateparseinfotable(lua_State* L) {
	struct parseinfo_table* t = (struct parseinfo_table*)skynet_malloc(sizeof(struct parseinfo_table));
	int i;
	for (i=0; i<HASHSIZE; ++i) {
		t->hash[i] = NULL;
	}
	lua_pushlightuserdata(L, t);
	return 1;
}

/* boxproto.freeparseinfotable(pt) */
static int
lfreeparseinfotable(lua_State* L) {
	struct parseinfo_table* t = lua_touserdata(L, 1);
	int i;
	for (i=0; i<HASHSIZE; ++i) {
		struct parseinfo* pi = t->hash[i];
		struct parseinfo* last = NULL;
		while (pi) {
			last = pi;
			pi = pi->next;
			skynet_free(last);
		}
	}
	skynet_free(t);
	return 0;
}

/* version = boxproto.bmversion(chainnode) */
static int 
lbmversion(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	uint8_t ver = boxmsg_version(&(chainnode->msg));
	lua_pushinteger(L, ver);
	return 1;
}

/* cmd = boxproto.bmcmd(chainnode) */
static int
lbmcmd(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	uint16_t cmd = boxmsg_cmd(&(chainnode->msg));
	lua_pushinteger(L, cmd);
	return 1;
}

/* flag = boxproto.bmflag(chainnode) */
static int
lbmflag(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	uint32_t flag = boxmsg_flag(&(chainnode->msg));
	lua_pushinteger(L, flag);
	return 1;
}

/* sid = boxproto.bmsid(chainnode) */
static int
lbmsid(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	uint32_t sid = boxmsg_sid(&(chainnode->msg));
	lua_pushinteger(L, sid);
	return 1;
}

/* u32 = boxproto.bmreadu32(chainnode) */
static int
lbmreadu32(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	int i = boxmsg_readu32(&(chainnode->msg));
	lua_pushinteger(L, i);
	return 1;
}

/* i32 = boxproto.bmreadi32(chainnode) */ 
static int 
lbmreadi32(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	int i = boxmsg_readi32(&(chainnode->msg));
	lua_pushinteger(L, i);
	return 1;
}

/* u16 = boxproto.bmreadu16(chainnode) */
static int
lbmreadu16(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	int i = boxmsg_readu16(&(chainnode->msg));
	lua_pushinteger(L, i);
	return 1;
}

/* i16 = boxproto.bmreadi16(chainnode) */
static int
lbmreadi16(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	int i = boxmsg_readu16(&(chainnode->msg));
	lua_pushinteger(L, i);
	return 1;
}

/* cstr = boxproto.bmreadcstring(chainnode) */
static int
lbmreadcstring(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	char* cstr = boxmsg_readcstring(&(chainnode->msg));
	lua_pushstring(L, cstr);
	return 1;
}

/* bin = boxproto.bmreadbinary(chainnode) */
static int
lbmreadbinary(lua_State* L) {
	struct boxmsg_chain* chainnode = lua_touserdata(L, 1);
	char buf[BOXMSGBUFCAP] = {0};
	int i = boxmsg_readbinary(&(chainnode->msg), buf, BOXMSGBUFCAP);
	lua_pushlstring(L, buf, i);
	return 1;
}

/* boxproto.freemsg(msg) */
static int 
lfreemsg(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	skynet_free(msg);
	return 0;
}

/* msg = boxproto.bmbegin(cmd) */
static int 
lbmbegin(lua_State* L) {
	int cmd = luaL_checkinteger(L, 1);
	struct boxmsg* msg = skynet_malloc(sizeof(struct boxmsg));
	boxmsg_begin(msg, cmd);
	lua_pushlightuserdata(L, msg);
	return 1;
}

/* boxproto.bmend(msg) */
static int
lbmend(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	boxmsg_end(msg);
	return 0;
}

/* boxproto.bmwriteu16(msg, i) */
static int
lbmwriteu16(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	int i = luaL_checkinteger(L, 2);
	boxmsg_writeu16(msg, i);
	return 0;
}

/* boxproto.bmwritei16(msg, i) */
static int
lbmwritei16(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	int i = luaL_checkinteger(L, 2);
	boxmsg_writei16(msg, i);
	return 0;
}

/* boxproto.bmwriteu32(msg, i) */
static int
lbmwriteu32(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	int i = luaL_checkinteger(L, 2);
	boxmsg_writeu32(msg, i);
	return 0;
}

/* boxproto.bmwritei32(msg, i) */
static int
lbmwritei32(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	int i = luaL_checkinteger(L, 2);
	boxmsg_writei32(msg, i);
	return 0;
}

/* boxproto.bmwritecstring(msg, cstr) */
static int
lbmwritecstring(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	char* cstr = (char*)lua_tostring(L, 2);
	boxmsg_writecstring(msg, cstr);
	return 0;
}

/* boxproto.bmwritebinary(msg, bin) */
static int
lbmwritebinary(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	size_t length = 0;
	const char* data = luaL_checklstring(L, 2, &length);
	boxmsg_writebinary(msg, data, length);
	return 0;
}

/* boxproto.bmsetversion(msg, ver) */
static int
lbmsetversion(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	uint8_t ver = luaL_checkinteger(L, 2);
	ver = endianopu8(ver);
	boxmsg_writehead(msg, (char*)&ver, sizeof(ver), 0);
	return 0;
}

/* boxproto.bmsetflag(msg, flag) */
static int
lbmsetflag(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	uint32_t flag = luaL_checkinteger(L, 2);
	flag = endianopu32(flag);
	boxmsg_writehead(msg, (char*)&flag, sizeof(flag), 4);
	return 0;
}

/* boxproto.bmsetsid(msg, sid) */
static int
lbmsetsid(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	uint32_t sid = luaL_checkinteger(L, 2);
	sid = endianopu32(sid);
	boxmsg_writehead(msg, (char*)&sid, sizeof(sid), 8);
	return 0;
}

/* lstr = boxproto.bmtolstring(msg) */
static int
lbmtolstring(lua_State* L) {
	struct boxmsg* msg = lua_touserdata(L, 1);
	lua_pushlstring(L, msg->buf, msg->size);
	return 1;
}

int
luaopen_boxproto(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "init", linit },
		{ "fini", lfini },
		{ "parse", lparse },
		{ "takefront", ltakefront },
		{ "freechainnode", lfreechainnode },
		{ "createparseinfotable", lcreateparseinfotable },
		{ "freeparseinfotable", lfreeparseinfotable },
		{ "bmversion", lbmversion },
		{ "bmcmd", lbmcmd },
		{ "bmflag", lbmflag },
		{ "bmsid", lbmsid },
		{ "bmreadu32", lbmreadu32 },
		{ "bmreadi32", lbmreadi32 },
		{ "bmreadu16", lbmreadu16 },
		{ "bmreadi16", lbmreadi16 },
		{ "bmreadcstring", lbmreadcstring },
		{ "bmreadbinary", lbmreadbinary },
		{ "freemsg", lfreemsg },
		{ "bmbegin", lbmbegin },
		{ "bmend", lbmend },
		{ "bmwriteu16", lbmwriteu16 },
		{ "bmwritei16", lbmwritei16 },
		{ "bmwriteu32", lbmwriteu32},
		{ "bmwritei32", lbmwritei32 },
		{ "bmwritecstring", lbmwritecstring },
		{ "bmwritebinary", lbmwritebinary },
		{ "bmsetversion", lbmsetversion },
		{ "bmsetflag", lbmsetflag },
		{ "bmsetsid", lbmsetsid },
		{ "bmtolstring", lbmtolstring },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}
