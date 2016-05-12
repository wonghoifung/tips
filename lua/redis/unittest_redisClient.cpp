#include "redisClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
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
static void check_bool(const char* tag, bool b, bool verbose=true) {
	if (b) {
		okcnt += 1;
		if (verbose) printf("----> %-10s ok\n", tag);
	} else {
		failurecnt += 1;
		if (verbose) printf("----> %-10s failure\n", tag);
	}
}

static bool in_vector(const std::string& s, const std::vector<std::string>& v) {
	for (size_t i=0; i<v.size(); ++i) {
		if (v[i]==s) return true;
	}
	return false;
}

static bool list_visit(const std::string& val) {
	printf("----> %s\n", val.c_str());
}

static bool hash_visit(const std::string& key, const std::string& val) {
	printf("----> %s: %s\n", key.c_str(), val.c_str());
}

static bool zset_visit(const std::string& val, int score) {
	printf("----> %s: %d\n", val.c_str(), score);
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

int main() {
	set_rlimit();

	timekeeper tk("main");

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
		check_bool("del", prc->del("mylist"));
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
	{
		check_bool("del", prc->del("myhash"));
		check_bool("hset", prc->hset("myhash", "id1", "123"));
		check_bool("hset", prc->hset("myhash", "id2", "456"));
		check_bool("hset", prc->hset("myhash", "id3", "789"));
		check_bool("hget", ("123"==prc->hget("myhash","id1")));
		check_bool("hget", ("456"==prc->hget("myhash","id2")));
		check_bool("hget", ("789"==prc->hget("myhash","id3")));
		check_bool("hget", (""==prc->hget("myhash","id4")));
		check_bool("hlen", (3==prc->hlen("myhash")));
		check_bool("hexists", (true==prc->hexists("myhash","id1")));
		check_bool("hexists", (true==prc->hexists("myhash","id2")));
		check_bool("hexists", (true==prc->hexists("myhash","id3")));
		check_bool("hexists", (false==prc->hexists("myhash","id4")));
		std::vector<std::string> keys;
		check_bool("hkeys", prc->hkeys("myhash", keys));
		check_bool("hkeys result", (3==keys.size() && in_vector("id1",keys) && in_vector("id2",keys) && in_vector("id3",keys)));
		std::map<std::string, std::string> res;
		check_bool("hgetall", prc->hgetall("myhash",res));
		check_bool("hgetall result", (3==res.size() && res["id1"]=="123" && res["id2"]=="456" && res["id3"]=="789"));
		check_bool("hvisit", prc->hvisit("myhash", hash_visit));
		std::vector<std::string> fields;
		fields.push_back("id2");
		check_bool("hdel", prc->hdel("myhash", fields));
		check_bool("hvisit", prc->hvisit("myhash", hash_visit));
		check_bool("hdel", prc->hdel("myhash", 1, "id1"));
		check_bool("hvisit", prc->hvisit("myhash", hash_visit));
	}
	{
		check_bool("del", prc->del("myset"));
		std::vector<std::string> members;
		members.push_back("a");
		members.push_back("b");
		members.push_back("c");
		members.push_back("x");
		members.push_back("y");
		members.push_back("z");
		check_bool("sadd",prc->sadd("myset",members));
		check_bool("sadd",prc->sadd("myset",3,"m","n","o"));
		check_bool("sismember",(true==prc->sismember("myset","a")));
		check_bool("sismember",(true==prc->sismember("myset","b")));
		check_bool("sismember",(true==prc->sismember("myset","c")));
		check_bool("sismember",(true==prc->sismember("myset","x")));
		check_bool("sismember",(true==prc->sismember("myset","y")));
		check_bool("sismember",(true==prc->sismember("myset","z")));
		check_bool("sismember",(true==prc->sismember("myset","m")));
		check_bool("sismember",(true==prc->sismember("myset","n")));
		check_bool("sismember",(true==prc->sismember("myset","o")));
		check_bool("sismember",(false==prc->sismember("myset","p")));
		check_bool("scard",(9==prc->scard("myset")));
		members.clear();
		members.push_back("a");
		members.push_back("x");
		members.push_back("m");
		check_bool("srem",prc->srem("myset",members));
		check_bool("scard",(6==prc->scard("myset")));
		check_bool("sismember",(false==prc->sismember("myset","a")));
		check_bool("sismember",(true==prc->sismember("myset","b")));
		check_bool("sismember",(true==prc->sismember("myset","c")));
		check_bool("sismember",(false==prc->sismember("myset","x")));
		check_bool("sismember",(true==prc->sismember("myset","y")));
		check_bool("sismember",(true==prc->sismember("myset","z")));
		check_bool("sismember",(false==prc->sismember("myset","m")));
		check_bool("sismember",(true==prc->sismember("myset","n")));
		check_bool("sismember",(true==prc->sismember("myset","o")));
		check_bool("sismember",(false==prc->sismember("myset","p")));
		check_bool("srem",prc->srem("myset",3,"b","y","n"));
		check_bool("scard",(3==prc->scard("myset")));
		check_bool("sismember",(false==prc->sismember("myset","a")));
		check_bool("sismember",(false==prc->sismember("myset","b")));
		check_bool("sismember",(true==prc->sismember("myset","c")));
		check_bool("sismember",(false==prc->sismember("myset","x")));
		check_bool("sismember",(false==prc->sismember("myset","y")));
		check_bool("sismember",(true==prc->sismember("myset","z")));
		check_bool("sismember",(false==prc->sismember("myset","m")));
		check_bool("sismember",(false==prc->sismember("myset","n")));
		check_bool("sismember",(true==prc->sismember("myset","o")));
		check_bool("sismember",(false==prc->sismember("myset","p")));
	}
	{
		check_bool("del", prc->del("myzet"));
		std::map<std::string, int> pairs;
		pairs["a"]=3;
		pairs["b"]=2;
		pairs["c"]=1;
		pairs["x"]=11;
		pairs["y"]=22;
		pairs["z"]=33;
		check_bool("zadd", prc->zadd("myzet",pairs));
		check_bool("zadd", prc->zadd("myzet",3,111,"mm",222,"nn",333,"oo"));
		check_bool("zcard", (9==prc->zcard("myzet")));
		std::vector<std::string> res1;
		check_bool("zrange", prc->zrange("myzet",0,-1,res1));
		check_bool("zrange result", (9==res1.size()));
		check_bool("zrange result", ("c"==res1[0]));
		check_bool("zrange result", ("b"==res1[1]));
		check_bool("zrange result", ("a"==res1[2]));
		check_bool("zrange result", ("x"==res1[3]));
		check_bool("zrange result", ("y"==res1[4]));
		check_bool("zrange result", ("z"==res1[5]));
		check_bool("zrange result", ("mm"==res1[6]));
		check_bool("zrange result", ("nn"==res1[7]));
		check_bool("zrange result", ("oo"==res1[8]));
		std::map<std::string,int> res2;
		check_bool("zrange_withscores", prc->zrange_withscores("myzet",0,-1,res2));
		check_bool("zrange_withscores result", (9==res2.size()));
		check_bool("zrange_withscores result", (3==res2["a"]));
		check_bool("zrange_withscores result", (2==res2["b"]));
		check_bool("zrange_withscores result", (1==res2["c"]));
		check_bool("zrange_withscores result", (11==res2["x"]));
		check_bool("zrange_withscores result", (22==res2["y"]));
		check_bool("zrange_withscores result", (33==res2["z"]));
		check_bool("zrange_withscores result", (111==res2["mm"]));
		check_bool("zrange_withscores result", (222==res2["nn"]));
		check_bool("zrange_withscores result", (333==res2["oo"]));
		check_bool("zvisit",prc->zvisit("myzet",0,-1,zset_visit));
		std::vector<std::string> members;
		members.push_back("a");
		members.push_back("x");
		members.push_back("mm");
		check_bool("zrem",prc->zrem("myzet",members));
		check_bool("zcard", (6==prc->zcard("myzet")));
		check_bool("zvisit",prc->zvisit("myzet",0,-1,zset_visit));
		check_bool("zrem",prc->zrem("myzet",3,"b","y","nn"));
		check_bool("zcard", (3==prc->zcard("myzet")));
		check_bool("zvisit",prc->zvisit("myzet",0,-1,zset_visit));
	}

	std::vector<std::string> datas;
	for (int i=0; i<10000; ++i) {
		char buf[64] = {0};
		sprintf(buf,"tag%d",i);
		datas.push_back(buf);
	}

	{
		timekeeper tk("set 10000");
		for (int i = 0; i < (int)datas.size(); ++i)
		{
			prc->setstr(datas[i],datas[i]);
		}
	}
	{
		timekeeper tk("get 10000");
		for (int i = 0; i < (int)datas.size(); ++i)
		{
			check_bool("get 10000", prc->getstr(datas[i])==datas[i], false);
		}
	}
	{
		timekeeper tk("hset 10000");
		for (int i = 0; i < (int)datas.size(); ++i)
		{
			prc->hset("testhash",datas[i],datas[i]);
		}
	}
	{
		timekeeper tk("hget 10000");
		for (int i = 0; i < (int)datas.size(); ++i)
		{
			check_bool("get 10000", prc->hget("testhash",datas[i])==datas[i], false);
		}
	}

	printf("ok count: %d, failure count: %d\n", okcnt, failurecnt);
	return 0;
}
