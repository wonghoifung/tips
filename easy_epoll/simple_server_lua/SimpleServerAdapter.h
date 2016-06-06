#ifndef SIMPLESERVERADAPTER_HEADER
#define SIMPLESERVERADAPTER_HEADER

#include "SimpleServer.h"
#include "luaport.h"
#include "skeleton.h"

class SimpleServerAdapter : public SimpleServer
{
public:
	SimpleServerAdapter():SimpleServer(skeleton_eventloop()) { // hack

	}
	int init(lua_State* L) {
		int port = (int)lua_tonumber(L, 2);
		bool ret = SimpleServer::init(port);
		lua_pushnumber(L, ret);
		return 1;
	}
    static mfunc_item<SimpleServerAdapter> mfuncs[2];
};

#endif

