#include "lua_skeleton.h"
#include "luaport.h"
#include "skeleton.h"

static int skeleton_eventloop_lua(lua_State* L) {
	lua_pushlightuserdata(L, (void*)skeleton_eventloop());
	return 1;
}

static int skeleton_init_lua(lua_State* L) {
	int maxfdcnt = lua_tointeger(L, -1);
	int loglvl = lua_tointeger(L, -2);
	const char* appname = lua_tostring(L, -3);

	bool res = skeleton_init(appname, loglvl, maxfdcnt);
	lua_pushinteger(L, res);
	return 1;
}

static int skeleton_run_lua(lua_State* L) {
	skeleton_run();
	return 0;
}

void register_lua_skeleton()
{
	lua_register(lua_state(), "skeleton_eventloop", skeleton_eventloop_lua);
	lua_register(lua_state(), "skeleton_init", skeleton_init_lua);
	lua_register(lua_state(), "skeleton_run", skeleton_run_lua);
}
