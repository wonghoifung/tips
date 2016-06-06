#include "skeleton.h"
#include "log.h"
#include "timer.h"
#include "eventloop.h"
#include <sys/resource.h>
#include <stdlib.h>
#include <signal.h>

static eventloop gevloop;

eventloop* skeleton_eventloop() { return &gevloop; }

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

static void handle_signal(int signum)
{
    if (signum == SIGTERM || signum == SIGUSR1 || signum == SIGKILL) {
        log_error("recv signal: %d,  exit event loop", signum);
        gevloop.stop = 1;
    } 
}

static void set_signal()
{
	signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    signal(SIGTERM, handle_signal);    	
    signal(SIGUSR1, handle_signal);
	signal(SIGKILL, handle_signal);
}

bool skeleton_init(const char* appname, int loglvl, int maxfdcnt)
{
	set_rlimit();
	set_signal();
	init_log(appname);
	set_log_level(loglvl);
	toggle_hex_level();
	init_timer();

	return gevloop.init(maxfdcnt);
}

void skeleton_run() { gevloop.run(); }
