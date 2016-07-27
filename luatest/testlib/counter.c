#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int counter(lua_State* L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	double val = lua_tonumber(L, lua_upvalueindex(1));
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_pushnumber(L, ++val);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_pushvalue(L, -1);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_replace(L, lua_upvalueindex(1));
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	return 1;
}

static int newCounter(lua_State* L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_pushnumber(L, 0);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_pushcclosure(L, &counter, 1);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	return 1;
}

int luaopen_testlib_counter(lua_State* L) {
	// printf("%s\n", __FUNCTION__);

	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	luaL_checkversion(L);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	luaL_Reg l[] = {
		{ "newCounter" , newCounter },
		{ NULL, NULL },
	};

	luaL_newlibtable(L, l);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	// luaL_checktype(L, -1, LUA_TTABLE);
	// luaL_checktype(L, -1, LUA_TTHREAD);
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	luaL_setfuncs(L, l, 0); // 0 means no upvalues
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	return 1;
}
