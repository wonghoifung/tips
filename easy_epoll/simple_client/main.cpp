#include "SimpleClient.h"
#include "eventloop.h"
#include "log.h"
#include "timer.h"
#include "skeleton.h"
#include <string>
#include <stdio.h>

int main() {
	if (!skeleton_init("SimpleClient", 7, 1024)) {
		log_error("cannot init skeleton");
		return -1;
	}

	SimpleClient* ms = new SimpleClient(skeleton_eventloop());
	
	ms->connect("127.0.0.1", "6464");

	skeleton_run();

	return 0;
}
