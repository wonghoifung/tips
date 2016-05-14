#include "timer.h"
#include <sys/resource.h>
#include <iostream>

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
	// set_rlimit();
	std::cout<<"0"<<std::endl;
	init_timer();
	std::cout<<"1"<<std::endl;
	time_ev te = {1,NULL,(void*)"albert",te_callback};
	std::cout<<"2"<<std::endl;
	start_timer(1, 1000, &te);
	std::cout<<"3"<<std::endl;
	start_timer(2, 1000, &te);
	std::cout<<"4"<<std::endl;
}
