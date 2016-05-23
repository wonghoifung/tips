#include "luaport.h"
#include "log.h"
#include "timer.h"
#include "SimpleServerAdapter.h"
#include "PeerAdapter.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

inline void set_rlimit()
{
	struct rlimit rlim, rlim_new;

	if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = 100000;
		if (setrlimit(RLIMIT_NOFILE, &rlim_new) != 0)
		{
			printf("set rlimit file failure\n");
			exit(0);
		}
	}

	if (getrlimit(RLIMIT_CORE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new) != 0)
		{
			printf("set rlimit core failure\n");
			exit(0);
		}
	}
}

int main() {
	set_rlimit();
	init_log("simple_server_lua");
	set_log_level(7);
	toggle_hex_level();
	init_timer();

	if (!lua_init()) {
		printf("cannot init lua\n");
		exit(1);
	}
	luaport<SimpleServerAdapter>::register_class(lua_state(), "SimpleServer");
	// luaport<PeerAdapter>::register_class(L, "Peer");
	lua_dofile("./scripts/main.lua");
	lua_fini();
	exit(0);
}
