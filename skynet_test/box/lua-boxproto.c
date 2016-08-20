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
	static inline int8_t endianopi8(int8_t v) { return v; }
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
	static inline int64_t endianopi64(int64_t v) { 
		int64_t result;
		((unsigned char*)&result)[0] = ((const unsigned char*)&v)[7];
		((unsigned char*)&result)[1] = ((const unsigned char*)&v)[6];
		((unsigned char*)&result)[2] = ((const unsigned char*)&v)[5];
		((unsigned char*)&result)[3] = ((const unsigned char*)&v)[4];
		((unsigned char*)&result)[4] = ((const unsigned char*)&v)[3];
		((unsigned char*)&result)[5] = ((const unsigned char*)&v)[2];
		((unsigned char*)&result)[6] = ((const unsigned char*)&v)[1];
		((unsigned char*)&result)[7] = ((const unsigned char*)&v)[0];
		return result;
	}
	static inline uint64_t endianopu64(uint64_t v) { 
		uint64_t result;
		((unsigned char*)&result)[0] = ((const unsigned char*)&v)[7];
		((unsigned char*)&result)[1] = ((const unsigned char*)&v)[6];
		((unsigned char*)&result)[2] = ((const unsigned char*)&v)[5];
		((unsigned char*)&result)[3] = ((const unsigned char*)&v)[4];
		((unsigned char*)&result)[4] = ((const unsigned char*)&v)[3];
		((unsigned char*)&result)[5] = ((const unsigned char*)&v)[2];
		((unsigned char*)&result)[6] = ((const unsigned char*)&v)[1];
		((unsigned char*)&result)[7] = ((const unsigned char*)&v)[0];
		return result;
	}
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
boxmsg_write(struct boxmsg* msg, const char* inbuf, int len) {
	if (msg->size < 0 || (len + p->size) > BOXMSGBUFCAP) return 0; // false
	memcpy(msg->buf + msg->size, inbuf, len);
	msg->size += len;
	return 1; // true
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
	if (t == NULL) {
		t = lua_newuserdata(L, sizeof(struct parseinfo_table));
		int i;
		for (i=0; i<HASHSIZE; ++i) {
			t->hash[i] = NULL;
		}
		lua_replace(L, 1);
	}
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
boxmsg_reset(boxmsg* msg) {
	msg->size = BOXMSGHEADSZ;
	msg->readpos = BOXMSGHEADSZ;
}

static void
parseinfo_reset(parseinfo* p) {
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
		p->msg[p->packpos++] = data[(*ndx)++];
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
parse_body(struct parseinfo* p, const char* data, const size_t length, size_t* pndx) {
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

/* chain, error = boxproto.parse(table, id, str) */
static int
lparse(lua_State* L) {
	struct parseinfo_table* t = get_parseinfo_table(L);
	int id = luaL_checkinteger(L, 2);
	size_t length = 0;
	const char* data = luaL_checklstring(L, 3, &length);
	struct parseinfo* p = find_parseinfo(t, id);
	assert(p);

	int msgcnt = 0;
	struct boxmsg_chain* chain = NULL;
	struct boxmsg_chain* last = NULL;
	size_t ndx = 0;
	while (ndx < sz && p->state != parse_error) {
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
				if (parse_body(p, data, length, &ndx)) {
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
				memcpy((char*)(chain->msg), p->msg.buf, p->msg.size);
				chain->next = NULL;
				last = chain;
			} else {
				last->next = (struct boxmsg_chain*)skynet_malloc(sizeof(*chain));
				memcpy((char*)(last->next->msg), p->msg.buf, p->msg.size);
				last->next->next = NULL;
				last = last->next;
			}
			msgcnt += 1;
			parseinfo_reset(p);
		}
	}
	if (chain == NULL) {
		lua_puahnil(L);
		lua_pushinteger(L, 0);
		return 2;
	} else {
		lua_pushlightuserdata(L, chain);
		lua_puahinteger(L, msgcnt);
		return 2;
	}
}

int
luaopen_boxproto(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "init", linit },
		{ "fini", lfini },
		{ "parse", lparse },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}
