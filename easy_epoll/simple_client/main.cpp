#include "SimpleClient.h"
#include "MuteServer.h"
#include "log.h"
#include "timer.h"
#include <sys/resource.h>
#include <string>
#include <stdio.h>

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
	init_log("SimpleServer");
	set_log_level(7);
	toggle_hex_level();
	init_timer();

	MuteServer ms;
	if (!ms.init_event()) {
		printf("cannot init event\n");
		exit(0);
	}
	
	SimpleClient* sc = new SimpleClient(&ms);
	sc->connect("127.0.0.1", "6464");

	ms.run();

	return 0;
}
