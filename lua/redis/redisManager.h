//
//  Created by huanghaifeng on 16-5-12.
//  Copyright (c) 2016 wonghoifung. All rights reserved.
//
#ifndef REDISMANAGER_HEADER
#define REDISMANAGER_HEADER

#include "redisClient.h"

class redisManager
{
	typedef std::map<std::string, std::vector<int> > svimap_t;
	redisManager(const redisManager&);
	redisManager& operator=(const redisManager&);
public:
	redisManager();
	~redisManager();
	bool addRedis(int id, const std::string& ip, int port);
	bool delRedis(int id);
	redisClient* getRedis(int id);
	bool add(const std::string& key, int id);
	bool del(const std::string& key);
	const std::vector<int>* get(const std::string& key) const;
	redisClient* getRedis(int h, const std::string& key);
private:
	std::map<int, redisClient*> redisClients_;
	svimap_t keyClients_;
};

#endif

