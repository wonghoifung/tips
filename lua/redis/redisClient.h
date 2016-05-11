//
//  Created by huanghaifeng on 16-5-11.
//  Copyright (c) 2016 wonghoifung. All rights reserved.
//
#ifndef REDISCLIENT_HEADER
#define REDISCLIENT_HEADER

#include <hiredis.h>
#include <string>
#include <vector>
#include <map>

class redisClient 
{
	typedef std::map<std::string, std::string> scriptmap_t;
	typedef bool (*list_visit_callback)(const std::string& val);
	typedef bool (*hash_visit_callback)(const std::string& key, const std::string& val);
	typedef bool (*zset_visit_callback)(const std::string& val, int score);

	redisClient(const redisClient&);
	redisClient& operator=(const redisClient&);

public:
	redisClient(const char* ip, uint16_t port);
	~redisClient();

	bool connect();
	bool disconnect();
	bool ping();
	bool keys(const std::string& pattern, std::vector<std::string>& result);
	bool keys_visit(const std::string& pattern, list_visit_callback cb);

	bool load_scripts(const char* spath);
	bool call_script(const std::string& scriptid, const std::vector<std::string>& keys, const std::vector<std::string>& args);
	bool call_script(const std::string& scriptid, int keycnt, int cnt, ...); // cnt is total of keys cnt and args cnt

	bool setstr(const std::string& key, const std::string& val);
	std::string getstr(const std::string& key);
	bool setint(const std::string& key, int val);
	int getint(const std::string& key);
	bool del(const std::string& key);

	bool lpush(const std::string& key, const std::vector<std::string>& vals);
	bool lpush(const std::string& key, int cnt, ...);
	std::string lpop(const std::string& key);
	bool rpush(const std::string& key, const std::vector<std::string>& vals);
	bool rpush(const std::string& key, int cnt, ...);
	std::string rpop(const std::string& key);
	int llen(const std::string& key);
	bool lrem(const std::string& key, int count, const std::string& val);
	bool lrange(const std::string& key, int start, int stop, std::vector<std::string>& result);
	bool lvisit(const std::string& key, int start, int stop, list_visit_callback cb);

	bool hgetall(const std::string& key, std::map<std::string, std::string>& result);
	bool hvisit(const std::string& key, hash_visit_callback cb);
	int hlen(const std::string& key);
	bool hkeys(const std::string& key, std::vector<std::string>& result);
	bool hexists(const std::string& key, const std::string& field);
	bool hset(const std::string& key, const std::string& field, const std::string& val);
	std::string hget(const std::string& key, const std::string& field);
	bool hdel(const std::string& key, const std::vector<std::string>& fields);
	bool hdel(const std::string& key, int cnt, ...);

	bool sadd(const std::string& key, const std::vector<std::string>& members);
	bool sadd(const std::string& key, int cnt, ...);
	bool srem(const std::string& key, const std::vector<std::string>& members);
	bool srem(const std::string& key, int cnt, ...);
	bool sismember(const std::string& key, const std::string& member);
	int scard(const std::string& key);

	bool zadd(const std::string& key, const std::map<std::string, int>& pairs);
	bool zadd(const std::string& key, int cnt, ...); // cnt pairs of score and value
	bool zrem(const std::string& key, const std::vector<std::string>& members);
	bool zrem(const std::string& key, int cnt, ...); // cnt is number of members
	int zcard(const std::string& key);
	bool zrange(const std::string& key, int start, int stop, std::vector<std::string>& result);
	bool zrange_withscores(const std::string& key, int start, int stop, std::map<std::string,int>& result);
	bool zvisit(const std::string& key, int start, int stop, zset_visit_callback cb);

private:
	std::string ip_;
	uint16_t port_;
	redisContext* ctx_;
	scriptmap_t scriptid2sha_;
};

#endif
