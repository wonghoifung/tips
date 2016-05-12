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

static int okcnt = 0;
static int failurecnt = 0;
static void check_bool(const char* tag, bool b) {
	if (b) {
		okcnt += 1;
		printf("----> %-10s ok\n", tag);
	} else {
		failurecnt += 1;
		printf("----> %-10s failure\n", tag);
	}
}

static bool list_visit(const std::string& val) {
	printf("----> %s\n", val.c_str());
}

int main() {
	set_rlimit();

	redisClient rc("127.0.0.1", 6379);
	redisClient* prc = &rc;
	check_bool("connect", prc->connect());
	check_bool("ping", prc->ping());
	check_bool("disconnect", prc->disconnect());
	check_bool("!ping", !prc->ping());
	check_bool("connect", prc->connect());
	check_bool("ping", prc->ping());
	check_bool("ping", prc->ping());
	std::vector<std::string> result;
	check_bool("keys", prc->keys("*", result));
	for (size_t i=0; i<result.size(); ++i) {
		printf("%s\n", result[i].c_str());
	}
	check_bool("keys_visit", prc->keys_visit("*", list_visit));
	check_bool("load_scripts", prc->load_scripts("./scripts")); // hash_json_set
	{
		std::vector<std::string> keys;
		keys.push_back("hjson");
		std::vector<std::string> args;
		args.push_back("id1");
		args.push_back("{\"name\":\"albert\", \"age\":31}");
		check_bool("call_script", prc->call_script("hash_json_set",NULL,NULL,keys,args));
	}
	{
		std::string out;
		std::vector<std::string> keys;
		keys.push_back("hjson");
		std::vector<std::string> args;
		args.push_back("id1");
		args.push_back("name");
		check_bool("call_script", prc->call_script("hash_json_get",&out,NULL,keys,args));
		check_bool("hash_json_get result", (out=="albert"));
	}
	{
		int age = 0;
		std::vector<std::string> keys;
		keys.push_back("hjson");
		std::vector<std::string> args;
		args.push_back("id1");
		args.push_back("age");
		check_bool("call_script", prc->call_script("hash_json_get",NULL,&age,keys,args));
		check_bool("hash_json_get result", (age==31));
	}
	check_bool("call_script", prc->call_script("hash_json_set",NULL,NULL,1,3,"hjson","id2","{\"name\": \"wong\", \"age\": 30}"));
	{
		int age = 0;
		check_bool("call_script", prc->call_script("hash_json_get",NULL,&age,1,3,"hjson","id2","age"));
		check_bool("hash_json_get result", (age==30));
	}
	check_bool("call_script", prc->call_script("hash_json_set",NULL,NULL,1,3,"hjson","id2","{\"age\":28, \"hobby\":\"skateboard\"}"));
	{
		int age = 0;
		check_bool("call_script", prc->call_script("hash_json_get",NULL,&age,1,3,"hjson","id2","age"));
		check_bool("hash_json_get result", (age==28));
		std::string hobby;
		check_bool("call_script", prc->call_script("hash_json_get",&hobby,NULL,1,3,"hjson","id2","hobby"));
		check_bool("hash_json_get result", (hobby=="skateboard"));
		std::string name;
		check_bool("call_script", prc->call_script("hash_json_get",&name,NULL,1,3,"hjson","id2","name"));
		check_bool("hash_json_get result", (name=="wong"));
	}

	printf("ok count: %d, failure count: %d\n", okcnt, failurecnt);
	return 0;
}
