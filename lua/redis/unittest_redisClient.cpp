#include "redisClient.h"
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

static void check_bool(const char* tag, bool b) {
	if (b) {
		printf("%-10s ok\n", tag);
	} else {
		printf("%-10s failure\n", tag);
	}
}

int main() {
	set_rlimit();

	redisClient rc("127.0.0.1", 6379);
	redisClient* prc = &rc;
	check_bool("connect", prc->connect());
	check_bool("ping", prc->ping());

	return 0;
}
