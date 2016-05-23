#include "luaport.h"
#include "message_adapter.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	if (!lua_init()) {
		printf("cannot init lua\n");
		exit(1);
	}

	luaport<lua_inmessage>::register_class(L, "inmsg");
	luaport<lua_outmessage>::register_class(L, "outmsg");

	lua_dofile("./scripts/main.lua");
	lua_fini();
	exit(0);
}
