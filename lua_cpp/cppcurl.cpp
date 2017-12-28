extern "C"
{

#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int http_post(lua_State* lua)
{
	printf("post\n");
	return 0;
}

static int http_get(lua_State* lua)
{
	printf("get\n");
	return 0;
}

int luaopen_cppcurl(lua_State* lua)
{
	luaL_Reg l[] = {
		{"http_post", http_post},
		{"http_get", http_get},
		{NULL, NULL}
	};
	luaL_newlib(lua, l);
	return 1;
}

}
