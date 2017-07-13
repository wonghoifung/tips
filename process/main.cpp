#include "process.h"
#include "env.h"
#include "signalop.h"

int main(int argc, char** argv)
{
	env_init();
	rlimit_init();
	signalop_init();
	proc_title_init(argv);

	process_init();
	master_run();
	worker_run();
}
