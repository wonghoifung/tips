#include "skeleton.h"
#include "log.h"
#include <signal.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>

aeEventLoop* gevloop = NULL;

static void signal_handler(int sig) {
	(void)sig;
    aeStop(gevloop);
}

static void set_signalhandler(void) {
	signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
}

static void set_rlimit()
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

aeEventLoop* skeleton_evloop() { return gevloop; }

bool skeleton_init(const char* logname, int loglevel, int maxfdcnt) {
	set_rlimit();
	set_signalhandler();
	init_log(logname);
	set_log_level(loglevel);
	toggle_hex_level();

	gevloop = aeCreateEventLoop(maxfdcnt);
	if (gevloop == NULL) {
		log_error("cannot create eventloop");
		return false;
	}
	return true;
}

void skeleton_run() {
	aeMain(gevloop);
	aeDeleteEventLoop(gevloop);
}
