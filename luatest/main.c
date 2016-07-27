#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize) {
	(void)ud;  (void)osize; 
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}

int main() {
	lua_State* L = lua_newstate(l_alloc, NULL);
	luaL_openlibs(L);
	int r = luaL_loadfile(L, "./test.lua");
	if (r != LUA_OK) {
		printf("cannot load file test.lua: %s\n", lua_tostring(L, -1));
		return 1;
	}
	r = lua_pcall(L, 0, 0, 0); // no args, no returns, no errmsg handler
	if (r != LUA_OK) {
		printf("lua_pcall error: %s\n", lua_tostring(L, -1));
		return 1;
	}
	lua_close(L);
	return 0;
}
