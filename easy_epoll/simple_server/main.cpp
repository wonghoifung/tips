#include "SimpleServer.h"
#include "log.h"
#include "timer.h"
#include "skeleton.h"
#include <string>
#include <stdio.h>

int main(int argc, char** argv) {
	if (!skeleton_init("SimpleServer", 7, 100000)) {
		log_error("init skeleton failed");
		return -1;
	}

	SimpleServer* ss = new SimpleServer(skeleton_eventloop());
	if (!ss->init(6464)) {
		log_error("init failed");
		return 0;
	}

	skeleton_run();

	delete ss;
	ss=NULL;

	return 0;
}