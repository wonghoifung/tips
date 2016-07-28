#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int l_hello(lua_State* l)
{
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(l));
	// int forward = lua_toboolean(l, 2);
	// printf("forward:%d\n", forward);
	
    printf("Hello, world!\n");
    return 0;
}

static int l_three(lua_State* l)
{
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(l));
	lua_pushinteger(l, 1);
	lua_pushinteger(l, 2);
	lua_pushinteger(l, 3);
	// lua_rotate(l, 1, 2); // from 1 to top, start from top, rotate 2 -> 2 3 1
	lua_rotate(l, 1, -1); // from 1 to top, start from bottom, rotate 1 -> 2 3 1
	// lua_rotate(l, 1, -2); // from 1 to top, start from bottom, rotate 2 -> 3 1 2
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(l));
	return 3;
}

static const struct luaL_Reg hello_lib[] = {
    {"hello", l_hello},
    {"three", l_three},
    {NULL, NULL},
};
 
int luaopen_testlib_luahello(lua_State* l)
{
    luaL_newlib(l, hello_lib);
    return 1;
}
