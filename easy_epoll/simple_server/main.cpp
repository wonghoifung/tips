#include "SimpleServer.h"
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

int main(int argc, char** argv) {
	set_rlimit();
	init_timer();
	init_log("SimpleServer");
	set_log_level(7);
	toggle_hex_level();

	SimpleServer* ss = new SimpleServer();
	if (!ss->InitSocket(6464)) {
		log_error("cannot start server socket");
		return 0;
	}
	if (!ss->init()) {
		log_error("init failed");
		return 0;
	}
	ss->Run();
	delete ss;
	ss=NULL;
	return 0;
}