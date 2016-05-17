#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <string>
#include <list>

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

class timekeeper {
	timekeeper(const timekeeper&);
	timekeeper& operator=(const timekeeper&);
	timespec begts;
	std::string info;
	long convertToMs(timespec& ts) {
		return ts.tv_sec*1000+ts.tv_nsec/1000000;
	}
public:
	timekeeper(const char* in) : info(in) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &begts);
	}
	~timekeeper() {
		timespec endts;
		clock_gettime(CLOCK_MONOTONIC_RAW, &endts);
		long diffms = convertToMs(endts) - convertToMs(begts);
		printf("[%s] time elasped: %ldms\n", info.c_str(), diffms);
	}
};

static int okcnt = 0;
static int failurecnt = 0;
static void test_log();

int main() {
	set_rlimit();
	timekeeper tk("main");
	test_log();
	TRACE("ok count: %d, failure count: %d\n", okcnt, failurecnt);
	return 0;
}

static void test_log() {
	init_log("unittest");
	set_log_level(7);
	toggle_hex_level();

	log_debug("dubug......%d",10);
	log_info("info......%d",12345);
	log_notice("notice......%f",5.5F);
	log_warning("warning......%s","bbbbbbbbbbb");
	log_error("error......%ld",987654L);
	log_crit("crit......%lf",999.1);
	log_alert("alert......%u",111111U);
	log_emerg("emerg......%d",100);
	TRACE("TRACE......%d",1000);

	int s = 0xABCDEF;
	hex_debug("hex",sizeof s,(char*)&s);

	const char* str = "abcdefghijklmnopqrstuvwxyz";
	hex_debug("hex",strlen(str),(char*)str);
}

static void check_bool(const char* tag, bool b, bool verbose=true) {
	if (b) {
		okcnt += 1;
		if (verbose) TRACE("[%s] -----> ok", tag);
	} else {
		failurecnt += 1;
		if (verbose) TRACE("[%s] -----> failure", tag);
	}
}


