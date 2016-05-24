#include "luaport.h"
#include "message_adapter.h"
#include "log.h"
#include "tcpconn.h"
#include <assert.h>

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
    lua_outmessage* msg = *(lua_outmessage**)lua_topointer(L, -1);
    if (msg == NULL) {
        printf("msg is null\n");
        lua_pushnumber(L, 1);
        return 1;
    }

    tcpconn* conn = (tcpconn*)lua_topointer(L, -2);
    if (conn == NULL) {
        printf("conn is null\n");
        lua_pushnumber(L, 1);
        return 1;
    }

    int ret = conn->sendbuf(msg->outmessage::buffer(), msg->outmessage::size());

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
