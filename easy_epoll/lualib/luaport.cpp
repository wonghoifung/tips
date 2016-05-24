#include "luaport.h"
#include "message_adapter.h"
#include "log.h"
#include "tcpconn.h"
#include <assert.h>

static int test_lua(lua_State* L) {
    const char* s = "hello";
    lua_getglobal(L, "test_c");
    lua_pushinteger(L, 7);
    lua_pushinteger(L, 8);
    lua_pushinteger(L, 9);
    // lua_pushlightuserdata(L, (void*)s);
    lua_pushstring(L, s);
    lua_call(L, 4, 1);
    // const char* ss = (const char*)lua_topointer(L, -1);
    const char* ss = lua_tostring(L, -1);
    lua_pop(L, 1);
    printf("ss:%s\n", ss);

    int i1 = (int)lua_tointeger(L,-1);
    int i2 = (int)lua_tointeger(L,-2);
    int i3 = (int)lua_tointeger(L,-3);
    printf("i1:%d i2:%d i3:%d\n", i1, i2, i3);
    return 0;
}

static int logdebug_lua(lua_State* L) {
	const char* content = (const char*)lua_tostring(L,-1);
	log_debug(content);
	return 0;
}

static int logerror_lua(lua_State* L) {
	const char* content = (const char*)lua_tostring(L,-1);
	log_error(content);
	return 0;
}

static int sendmsg_lua(lua_State* L) {

    printf("argc: %d\n", lua_gettop(L));

    // int siz = (int)lua_tointeger(L, -1);
    // printf("siz : %d\n", siz);
    printf("lua_isuserdata: %d\n", lua_isuserdata(L,-1));
    printf("lua_istable: %d\n", lua_istable(L,-1));

    lua_outmessage* msg = *(lua_outmessage**)lua_topointer(L, -1);
    if (msg == NULL) {
        printf("msg is null\n");
        lua_pushnumber(L, 1);
        return 1;
    }
    printf("msg size: %d\n", msg->outmessage::size());
    {
        if (msg->outmessage::buffer() == NULL) {
            printf("inm buffer null\n");
            return 0;
        }
        inmessage inm;
        inm.copy(msg->outmessage::buffer(), msg->outmessage::size());
        std::string s = inm.read_cstring();
        printf("c get: %s\n", s.c_str());
    }
    // const char* buf = (const char*)lua_topointer(L, -2);
    // if (buf == NULL) {
    //     printf("buf is null\n");
    //     lua_pushnumber(L, 1);
    //     return 1;
    // }

    tcpconn* conn = (tcpconn*)lua_topointer(L, -2);
    if (conn == NULL) {
        printf("conn is null\n");
        lua_pushnumber(L, 1);
        return 1;
    }

    int ret = conn->sendbuf(msg->outmessage::buffer(), msg->outmessage::size());
    // int ret = conn->sendbuf(buf, siz);

    lua_pushnumber(L, ret);

    return 1;
}

extern "C"
{
int dofile (lua_State *L, const char *name);
};

static lua_State* L = NULL;

lua_State* lua_state() {
    assert(L);
    return L;
}

bool lua_init() {
	if (L) {
        return false;
    }
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "test", test_lua);
    lua_register(L, "log_debug", logdebug_lua);
    lua_register(L, "log_error", logerror_lua);
    lua_register(L, "sendmsg", sendmsg_lua);

    luaport<lua_inmessage>::register_class(L, "inmsg");
	luaport<lua_outmessage>::register_class(L, "outmsg");

    return true;
}

bool lua_fini() {
    if (L==NULL) {
        return false;
    }
    lua_close(L);
    L = NULL;
    return true;
}

void lua_dofile(const char* fname) {
    assert(L);
    dofile(L, fname);
}
