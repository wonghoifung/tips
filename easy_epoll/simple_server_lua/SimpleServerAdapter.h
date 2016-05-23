#ifndef SIMPLESERVERADAPTER_HEADER
#define SIMPLESERVERADAPTER_HEADER

#include "SimpleServer.h"
#include "luaport.h"

class SimpleServerAdapter : public SimpleServer
{
public:
	int init_server(lua_State* L) {
		int port = (int)lua_tonumber(L, 2);
		bool ret = SimpleServer::init_server(port);
		lua_pushnumber(L, ret);
		return 1;
	}
	int init(lua_State* L) {
		bool ret = SimpleServer::init();
		lua_pushnumber(L, ret);
		return 1;
	}
	int run(lua_State* L) {
		SimpleServer::run();
		return 0;
	}
    static mfunc_item<SimpleServerAdapter> mfuncs[4];
};

#endif

