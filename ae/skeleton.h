#ifndef SKELETON_HEADER
#define SKELETON_HEADER

extern "C" {
#include "ae.h"
#include "anet.h"
}

aeEventLoop* skeleton_evloop();
bool skeleton_init(const char* logname, int loglevel, int maxfdcnt);
void skeleton_run();

#endif
