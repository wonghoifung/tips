#include "luaport.h"
#include "log.h"
#include "timer.h"
#include "SimpleServerAdapter.h"
#include "lua_skeleton.h"
#include "lua_timer.h"
#include "Peer.h"
#include <stdio.h>
#include <stdlib.h>

static int getpeerid_lua(lua_State* L) {
	tcpconn* conn = (tcpconn*)lua_topointer(L, -1);

	Peer* peer = getPeer(conn);

	lua_pushinteger(L, peer->getPeerId());
	return 1;
}

int main() {
	if (!lua_init()) {
		printf("cannot init lua\n");
		exit(1);
	}
	
	register_lua_skeleton();
	register_lua_timer();
	
	luaport<SimpleServerAdapter>::register_class(lua_state(), "SimpleServer");
	lua_register(lua_state(), "getpeerid", getpeerid_lua);

	lua_dofile("./scripts/main.lua");
	
	lua_fini();
	
	exit(0);
}
