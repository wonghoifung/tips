//
//  Created by huanghaifeng on 16-5-12.
//  Copyright (c) 2016 wonghoifung. All rights reserved.
//
#include "redisManager.h"

redisManager::redisManager() {

}

redisManager::~redisManager() {

}

bool redisManager::addRedis(int id, const std::string& ip, int port) {
	std::map<int, redisClient*>::iterator it = redisClients_.find(id);
	if (it != redisClients_.end()) {
		return false;
	} 
	redisClient* client = new redisClient(ip.c_str(), port);
	if (client == NULL) return false;
	if (!client->connect()) {
		delete client;
		return false;
	}
	redisClients_[id] = client;
	return true;
}

bool redisManager::delRedis(int id) {
	std::map<int, redisClient*>::iterator it = redisClients_.find(id);
	if (it != redisClients_.end()) {
		delete it->second;
		redisClients.erase(it);
	} 
	return true;
}

redisClient* redisManager::getRedis(int id) {
	std::map<int, redisClient*>::iterator it = redisClients_.find(id);
	if (it != redisClients_.end()) {
		return it->second;
	}
	return NULL;
}

bool redisManager::add(const std::string& key, int id) {
	svimap_t::iterator it = keyClients_.find(key);
	if (it != keyClients_.end()) {
		it->second.push_back(id);
	} else {
		keyClients_[key] = std::vector<int>(1, id);
	}
	return true;
}

bool redisManager::del(const std::string& key) {
	keyClients_.erase(key);
	return true;
}

const std::vector<int>* redisManager::get(const std::string& key) const {
	svimap_t::iterator it = keyClients_.find(key);
	if (it != keyClients_.end()) {
		return &it->second;
	}
	return NULL;
}

redisClient* redisManager::getRedis(int h, const std::string& key) {
	svimap_t::iterator it = keyClients_.find(key);
	if (it != keyClients_.end() && !it->second.empty()) {
		int id = it->second[h % (it->second.size())];
		return getRedis(id);
	}
	return NULL;
}
