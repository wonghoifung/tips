#ifndef SKELETON_HEADER
#define SKELETON_HEADER

class eventloop;
eventloop* skeleton_eventloop();
bool skeleton_init(const char* appname, int loglvl, int maxfdcnt);
void skeleton_run();

#endif

