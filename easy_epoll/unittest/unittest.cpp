#include "log.h"
#include "llist.h"
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
static void test_llist();

int main() {
	set_rlimit();
	timekeeper tk("main");
	test_log();
	test_llist();
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

static int list_find(ELE* pELE, void* key) {
	ELE* ekey = (ELE*)key;
	if (pELE->i == ekey->i && pELE->j == ekey->j && pELE->k == ekey->k) return 1;
	return 0;
}

static void list_travel(ELE* pELE, void* arg) {
	TRACE("[travel] %s i:%d j:%d", (char*)arg, pELE->i, pELE->j);
}

static void stdlist_delete(std::list<ELE*>& sl, list_find_p find, void* key) {
	std::list<ELE*>::iterator it = sl.begin();
	for (; it != sl.end(); ) {
		if (find(*it, key) == 1) sl.erase(it++);
		else ++it;
	}
}

static void test_llist() {
	LLIST* ll = llist_create();
	check_bool("llist size", llist_size(ll)==0);
	ELE e1 = {1,2,NULL};
	ELE e2 = {2,3,NULL};
	ELE e3 = {3,4,NULL};
	check_bool("llist append", llist_append(ll, &e1)==0);
	check_bool("llist append", llist_append(ll, &e2)==0);
	check_bool("llist append", llist_append(ll, &e3)==0);
	check_bool("llist size", llist_size(ll)==3);
	llist_travel(ll, list_travel, (void*)"test1");
	ELE e4 = {4,5,NULL};
	ELE e5 = {5,6,NULL};
	ELE e6 = {6,7,NULL};
	check_bool("llist preappend", llist_preappend(ll, &e4)==0);
	check_bool("llist preappend", llist_preappend(ll, &e5)==0);
	check_bool("llist preappend", llist_preappend(ll, &e6)==0);
	check_bool("llist size", llist_size(ll)==6);
	llist_travel(ll, list_travel, (void*)"test2");
	ELE e7 = {7,8,NULL};
	check_bool("llist delete", llist_delete(ll, list_find, &e2)==1);
	check_bool("llist delete", llist_delete(ll, list_find, &e4)==1);
	check_bool("llist delete", llist_delete(ll, list_find, &e7)==0);
	check_bool("llist size", llist_size(ll)==4);
	llist_travel(ll, list_travel, (void*)"test3");
	llist_destroy(ll);

	int cnt=10000;
	ELE datas[cnt];
	for (int i=0; i<cnt; ++i) {
		datas[i].i = i;
		datas[i].j = i + 1;
		datas[i].k = NULL;
	}

	{
		timekeeper tk("llist insert perf");
		LLIST* ll = llist_create();
		for (int i=0; i<cnt; ++i) {
			llist_append(ll, &datas[i]);
		}
		llist_destroy(ll);
	}

	{
		timekeeper tk("std::list insert perf");
		{
			std::list<ELE*> sl;
			for (int i=0; i<cnt; ++i) {
				sl.push_back(&datas[i]);
			}
		}
	}

	{
		LLIST* ll = llist_create();
		for (int i=0; i<cnt; ++i) {
			llist_append(ll, &datas[i]);
		}
		{
			timekeeper tk("llist size perf");
			for (int i=0; i<cnt; ++i) llist_size(ll);
		}
		llist_destroy(ll);
	}

	{
		std::list<ELE*> sl;
		for (int i=0; i<cnt; ++i) {
			sl.push_back(&datas[i]);
		}
		{
			timekeeper tk("std::list size perf");
			for (int i=0; i<cnt; ++i) sl.size();
		}
	}

	{
		LLIST* ll = llist_create();
		for (int i=0; i<cnt; ++i) {
			llist_append(ll, &datas[i]);
		}
		{
			timekeeper tk("llist delete perf");
			for (int i=cnt-1; i>=0; --i) llist_delete(ll,list_find,&datas[i]);
		}
		check_bool("llist size", 0==llist_size(ll));
		llist_destroy(ll);
	}

	{
		LLIST* ll = llist_create();
		for (int i=0; i<cnt; ++i) {
			llist_append(ll, &datas[i]);
		}
		{
			timekeeper tk("llist reverse delete perf");
			for (int i=0; i<cnt; ++i) llist_delete(ll,list_find,&datas[i]);
		}
		check_bool("llist size", 0==llist_size(ll));
		llist_destroy(ll);
	}

	{
		std::list<ELE*> sl;
		for (int i=0; i<cnt; ++i) {
			sl.push_back(&datas[i]);
		}
		{
			timekeeper tk("std::list delete perf");
			for (int i=cnt-1; i>=0; --i) stdlist_delete(sl,list_find,&datas[i]);
		}
		check_bool("std::list size", 0==sl.size());
	}

	{
		std::list<ELE*> sl;
		for (int i=0; i<cnt; ++i) {
			sl.push_back(&datas[i]);
		}
		{
			timekeeper tk("std::list reverse delete perf");
			for (int i=0; i<cnt; ++i) stdlist_delete(sl,list_find,&datas[i]);
		}
		check_bool("std::list size", 0==sl.size());
	}
}

