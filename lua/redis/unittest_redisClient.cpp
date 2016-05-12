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
		std::string sex;
		check_bool("call_script", prc->call_script("hash_json_get",&sex,NULL,1,3,"hjson","id2","sex"));
		check_bool("hash_json_get result", (sex==""));
	}
	check_bool("setstr", prc->setstr("a","axxxxxxxxxxxxxxxxxxxxxxxxxz"));
	check_bool("getstr", ("axxxxxxxxxxxxxxxxxxxxxxxxxz"==prc->getstr("a")));
	check_bool("setint", prc->setint("b",123456));
	check_bool("getint", (123456==prc->getint("b")));
	check_bool("del", prc->del("a"));
	check_bool("getstr", (""==prc->getstr("a")));
	check_bool("del", prc->del("b"));
	check_bool("getint", (0==prc->getint("b")));
	{
		check_bool("llen", (0==prc->llen("mylist")));
		std::vector<std::string> v;
		v.push_back("a");
		v.push_back("b");
		v.push_back("c");
		check_bool("lpush", prc->lpush("mylist",v));
		check_bool("lpush", prc->lpush("mylist", 2, "d", "e"));
		check_bool("llen", (5==prc->llen("mylist")));
		check_bool("lpop", ("e"==prc->lpop("mylist")));
		check_bool("llen", (4==prc->llen("mylist")));
		check_bool("lpop", ("d"==prc->lpop("mylist")));
		check_bool("llen", (3==prc->llen("mylist")));
		check_bool("lpop", ("c"==prc->lpop("mylist")));
		check_bool("llen", (2==prc->llen("mylist")));
		check_bool("rpop", ("a"==prc->rpop("mylist")));
		std::vector<std::string> vv;
		vv.push_back("x");
		vv.push_back("y");
		check_bool("rpush", prc->rpush("mylist",vv));
		check_bool("rpush", prc->rpush("mylist", 2, "z", "zz"));
		check_bool("llen", (5==prc->llen("mylist")));
		check_bool("rpop", ("zz"==prc->rpop("mylist")));
		check_bool("lrem", prc->lrem("mylist",0,"x"));
		check_bool("lrem", prc->lrem("mylist",0,"y"));
		std::vector<std::string> res;
		check_bool("lrange", prc->lrange("mylist",0,-1,res));
		check_bool("lrange result", (res.size()==2 && res[0]=="b" && res[1]=="z"));
		check_bool("lvisit", prc->lvisit("mylist",0,-1,list_visit));
	}

	printf("ok count: %d, failure count: %d\n", okcnt, failurecnt);
	return 0;
}
