#include "luaport.h"
#include <assert.h>

extern "C"
{
int dofile (lua_State *L, const char *name);
};

static lua_State* L = NULL;

lua_State* lua_state() {
	assert(L);
	return L;
}

bool lua_init() {
	if (L) {
        return false;
    }
    L = luaL_newstate();
    luaL_openlibs(L);
    return true;
}

bool lua_fini() {
    if (L==NULL) {
        return false;
    }
    lua_close(L);
    L = NULL;
    return true;
}

void lua_dofile(const char* fname) {
    assert(L);
    dofile(L, fname);
}
