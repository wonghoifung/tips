#include <stdio.h>
#include <sstream>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

int c_fun1(lua_State* lua)
{
	if (lua_type(lua, 1) == LUA_TSTRING)
	{
		printf("ok first argument\n");
	}
	luaL_checktype(lua, 1, LUA_TSTRING);
	luaL_checktype(lua, 2, LUA_TNUMBER);
	luaL_checktype(lua, 3, LUA_TBOOLEAN);
	luaL_checktype(lua, 4, LUA_TTABLE);
	luaL_checktype(lua, 5, LUA_TTABLE);
	size_t len;
	const char* s = lua_tolstring(lua, 1, &len);
	int i = lua_tonumber(lua, 2);
	bool b = lua_toboolean(lua, 3);
	int sz = luaL_len(lua, 4);
	std::stringstream ss;
	printf("#### %d\n", lua_gettop(lua));
	for (int i=1; i<=sz; ++i)
	{
		lua_pushnumber(lua, i);
		lua_gettable(lua, 4);
		const char* element = lua_tostring(lua, -1) ;
		ss << element << " ";
		lua_pop(lua, 1);
	}
	printf("#### %d\n", lua_gettop(lua));
	std::stringstream sss;
	lua_pushnil(lua);
	while (lua_next(lua, 5) != 0)
	{
		sss << lua_tostring(lua, -2) << "=";
		if (lua_isstring(lua, -1)) sss << lua_tostring(lua, -1) << " ";
		else sss << (int)lua_tonumber(lua, -1) << " ";
		lua_pop(lua, 1);
	}
	lua_getfield(lua, 5, "name");
	const char* name = lua_tostring(lua, -1);
	lua_pop(lua, 1);
	printf("name:%s\n", name);
	printf("#### %d\n", lua_gettop(lua));
	printf("%s(%s,%d,%d,{%s},{%s})\n", __FUNCTION__, s, i, b, ss.str().c_str(), sss.str().c_str());
	lua_pushstring(lua, "ccc");
	return 1;
}

int c_fun2(lua_State* lua)
{
	lua_newtable(lua);
	lua_pushstring(lua, "name");
	lua_pushstring(lua, "milk");
	lua_settable(lua, -3);
	lua_pushstring(lua, "age");
	lua_pushinteger(lua, 1);
	lua_settable(lua, -3);
	return 1;
}

int main()
{
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);

	lua_register(lua, "c_fun1", c_fun1);
	lua_register(lua, "c_fun2", c_fun2);

	lua_pushstring(lua, "hello");
	lua_setglobal(lua, "msg");

	lua_newtable(lua);
	lua_pushstring(lua, "name"); // key
	lua_pushstring(lua, "wong"); // value
	lua_settable(lua, -3);
	lua_pushstring(lua, "age");
	lua_pushinteger(lua, 28);
	lua_settable(lua, -3);
	lua_setglobal(lua, "wo");

	if (luaL_loadfile(lua, "test.lua"))
	{
		const char* error = lua_tostring(lua, -1);
		printf("lua load error: %s\n", error);
		lua_close(lua);
		return -1;
	}

	if (lua_pcall(lua, 0, 0, 0))
	{
		const char* error = lua_tostring(lua, -1);
		printf("lua call file error: %s\n", error);
		lua_close(lua);
		return -1;
	}

	lua_getglobal(lua, "width");
	int width = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	printf("width %d\n", width);

	lua_getglobal(lua, "ta");
	lua_getfield(lua, -1, "name");
	printf("ta.name %s\n", lua_tostring(lua, -1));
	lua_pop(lua, 1);
	lua_getfield(lua, -1, "age");
	printf("ta.age %d\n", (int)lua_tonumber(lua, -1));
	lua_pop(lua, 2);

	lua_getglobal(lua, "ferror");
	int ferrorIdx = lua_gettop(lua);
	lua_getglobal(lua, "onEvent");
	lua_pushstring(lua, "key");
	lua_newtable(lua);
	lua_pushstring(lua, "name");
	lua_pushstring(lua, "jin");
	lua_settable(lua, -3);
	lua_pushstring(lua, "age");
	lua_pushinteger(lua, 18);
	lua_settable(lua, -3);
	if (lua_pcall(lua, 2, 1, ferrorIdx) != 0)
	{
		const char* error = lua_tostring(lua, -1);
		printf("lua call function error: %s\n", error);
		lua_close(lua);
		return -1;
	}
	// const char* res = lua_tostring(lua, -1);
	lua_getfield(lua, -1, "ret");
	const char* res = lua_tostring(lua, -1);
	lua_pop(lua, 1);
	lua_pop(lua, 2); // include ferror
	printf("ret: %s\n", res);

	printf("top is %d\n", lua_gettop(lua));

	lua_close(lua);

	return 0;
}
