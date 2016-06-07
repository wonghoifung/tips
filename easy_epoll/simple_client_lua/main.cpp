#include "SimpleClient.h"
#include "eventloop.h"
#include "log.h"
#include "timer.h"
#include "skeleton.h"
#include "luaport.h"
#include <string>
#include <stdio.h>

static int simpleclient_create_lua(lua_State* L) {
	SimpleClient* mc = new SimpleClient(skeleton_eventloop());
	lua_pushlightuserdata(L, (void*)mc);
	return 1;
}

static int simpleclient_destroy_lua(lua_State* L) {
	SimpleClient* mc = (SimpleClient*)lua_topointer(L, -1);
	delete mc;
	return 0;
}

static int simpleclient_connect_lua(lua_State* L) {
	SimpleClient* mc = (SimpleClient*)lua_topointer(L, -3);
	const char* ip = lua_tostring(L, -2);
	const char* port = lua_tostring(L, -1);
	mc->connect(ip, port);
	return 0;
}

int main() {
	if (!lua_init()) {
		printf("cannot init lua\n");
		exit(1);
	}
	
	lua_register(lua_state(), "simpleclient_create", simpleclient_create_lua);
	lua_register(lua_state(), "simpleclient_destroy", simpleclient_destroy_lua);
	lua_register(lua_state(), "simpleclient_connect", simpleclient_connect_lua);

	lua_dofile("./scripts/main.lua");
	
	lua_fini();
}
