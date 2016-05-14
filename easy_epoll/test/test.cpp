#include "timer.h"
#include <sys/resource.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

void te_callback(void* ptr) {
	std::cout << (char*)ptr << std::endl;
}

int main() {
	set_rlimit();
	init_timer();
	time_ev te = {1,NULL,(void*)"albert",te_callback};
	start_timer(1, 1000, &te);
	start_timer(2, 1000, &te);
}
