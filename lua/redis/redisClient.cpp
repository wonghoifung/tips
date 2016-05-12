//
//  Created by huanghaifeng on 16-5-11.
//  Copyright (c) 2016 wonghoifung. All rights reserved.
//
#include "redisClient.h"
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>
#include <dirent.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>

#define CHECK_CONTEXT_RETBOOL do { \
	if (!ctx_) { \
		printf("redis not connect\n"); \
		return false; \
	} \
} while (0)

#define CHECK_CONTEXT_RETSTRING do { \
	if (!ctx_) { \
		printf("redis not connect\n"); \
		return ""; \
	} \
} while (0)

#define CHECK_CONTEXT_RETINT do { \
	if (!ctx_) { \
		printf("redis not connect\n"); \
		return 0; \
	} \
} while (0)

#define CHECK_REPLY_RETBOOL(reply) do { \
	if (!reply) { \
		printf("null reply: %s\n", ctx_->errstr); \
		return false; \
	} \
} while (0)

#define CHECK_REPLY_RETSTRING(reply) do { \
	if (!reply) { \
		printf("null reply: %s\n", ctx_->errstr); \
		return ""; \
	} \
} while (0)

#define CHECK_REPLY_RETINT(reply) do { \
	if (!reply) { \
		printf("null reply: %s\n", ctx_->errstr); \
		return 0; \
	} \
} while (0)

#define CHECK_REPLYTYPE_RETBOOL(reply, t) do { \
	if (reply->type != t) { \
		printf("error type: %d\n", reply->type); \
		freeReplyObject(reply); \
		return false; \
	} \
} while (0)

#define CHECK_REPLYTYPE_RETSTRING(reply, t) do { \
	if (reply->type != t) { \
		printf("error type: %d\n", reply->type); \
		freeReplyObject(reply); \
		return ""; \
	} \
} while (0)

#define CHECK_REPLYTYPE_RETINT(reply, t) do { \
	if (reply->type != t) { \
		printf("error type: %d\n", reply->type); \
		freeReplyObject(reply); \
		return 0; \
	} \
} while (0)

#define GET_REPLY_STRING(reply, buf) \
	buf = (char*)alloca(reply->len + 1); \
	memcpy(buf, reply->str, reply->len); \
	buf[reply->len] = 0

redisClient::redisClient(const char* ip, uint16_t port) : ip_(ip), port_(port), ctx_(NULL) {
	
}

redisClient::~redisClient() {
	disconnect();
}

bool redisClient::connect() {
	struct timeval timeout = { 1, 500000 };
	ctx_ = redisConnectWithTimeout(ip_.c_str(), port_, timeout);
	if (ctx_ == NULL || ctx_->err) {
		if (ctx_) {
			printf("connction error: %s\n", ctx_->errstr);
			redisFree(ctx_);
			ctx_ = NULL;
			return false;
		}
		printf("connction error: can't allocate redis context\n");
		return false;
	}
	return true;
}

bool redisClient::disconnect() {
	if (ctx_) {
		redisFree(ctx_);
		ctx_ = NULL;
	}
	return true;
}

bool redisClient::ping() {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "PING");
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_STATUS);
	char* buf = NULL;
	GET_REPLY_STRING(reply, buf);
	if (strcmp(buf, "PONG") != 0) {
		printf("error reply: %s\n", buf);
		freeReplyObject(reply);
		return false;
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::keys(const std::string& pattern, std::vector<std::string>& result) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "KEYS %s", pattern.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i = 0; i < reply->elements; ++i) {
		redisReply* r = reply->element[i];
		if (r->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);
			result.push_back(buf);
		} else {
			printf("unexpected type: %d\n", r->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::keys_visit(const std::string& pattern, list_visit_callback cb) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "KEYS %s", pattern.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i = 0; i < reply->elements; ++i) {
		redisReply* r = reply->element[i];
		if (r->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);
			cb(buf);
		} else {
			printf("unexpected type: %d\n", r->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::load_scripts(const char* spath) {
	CHECK_CONTEXT_RETBOOL;
	if (!scriptid2sha_.empty()) scriptid2sha_.clear();
		struct dirent* ptr;
	DIR* dir;
	dir = opendir(spath);
	if (dir == NULL) {
		printf("cannot open %s: %s\n", spath, strerror(errno));
		return false;
	}
	while ((ptr = readdir(dir)) != NULL) {
		if (ptr->d_name[0] == '.') continue;
		std::string filename = ptr->d_name;
		std::string scriptid = filename.substr(0, filename.size() - 4);
		std::string filepath = std::string(spath) + "/" + filename;
		std::ifstream in(filepath.c_str(), std::ios::in);
		std::istreambuf_iterator<char> beg(in), end;
		std::string script(beg, end);
		in.close();
		redisReply* reply = (redisReply*)redisCommand(ctx_, "script load %b", script.c_str(), script.size());
		if (!reply) {
			printf("error: %s\n", ctx_->errstr);
			closedir(dir);
			return false;
		}
		if (reply->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(reply, buf);
			printf("new script, id:%s, sha:%s\n", scriptid.c_str(), buf);
			scriptid2sha_[scriptid] = buf;
			freeReplyObject(reply);
		}
		else {
			printf("error reply->type: %d\n", reply->type);
			freeReplyObject(reply);
			closedir(dir);
			return false;
		}
	}
	closedir(dir);
	return true;
}

static redisReply* redisEval(redisContext* ctx, const std::string& sha, const std::vector<std::string>& keys, const std::vector<std::string>& args) {
	redisReply* reply = NULL;

	// evalsha sha keycnt keys... args...
	int argc = 1 + 1 + 1 + keys.size() + args.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = "EVALSHA";
	char* psha = (char*)alloca(sha.size() + 1); std::copy(sha.begin(), sha.end(), psha); psha[sha.size()] = 0;
	argv[idx++] = psha;
	char pkeycnt[32] = { 0 };
	snprintf(pkeycnt, sizeof pkeycnt, "%zu", keys.size());
	argv[idx++] = pkeycnt;
	for (size_t i = 0; i < keys.size(); ++i) {
		char* key = (char*)alloca(keys[i].size() + 1); std::copy(keys[i].begin(), keys[i].end(), key); key[keys[i].size()] = 0;
		argv[idx++] = key;
	}
	for (size_t i = 0; i < args.size(); ++i) {
		char* arg = (char*)alloca(args[i].size() + 1); std::copy(args[i].begin(), args[i].end(), arg); arg[args[i].size()] = 0;
		argv[idx++] = arg;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::call_script(const std::string& scriptid, std::string* outs, int* outi, const std::vector<std::string>& keys, const std::vector<std::string>& args) {
	CHECK_CONTEXT_RETBOOL;
	scriptmap_t::iterator it = scriptid2sha_.find(scriptid);
	if (it == scriptid2sha_.end()) {
		printf("cannot find scriptid %s\n", scriptid.c_str());
		return false;
	}
	redisReply* reply = NULL;
	reply = redisEval(ctx_, it->second, keys, args);
	if (!reply) {
		printf("eval error: %s\n", ctx_->errstr);
		return false;
	}
	if (reply->type == REDIS_REPLY_STRING) {
		char* buf = NULL;
		GET_REPLY_STRING(reply, buf);
		if (outs) {
			*outs = buf;
		} else {
			printf("unhandled: %s\n", buf);
		}
	} 
	else if (reply->type == REDIS_REPLY_INTEGER) {
		if (outi) {
			*outi = reply->integer;
		} 
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::call_script(const std::string& scriptid, std::string* outs, int* outi, int keycnt, int cnt, ...) { // cnt is total of keys cnt and args cnt
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> keys;
	std::vector<std::string> args;
	va_list vl;
	va_start(vl, cnt);
	int k(0), i(0);
	for (; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		if (k < keycnt) {
			keys.push_back(p);
			k += 1;
		} else {
			args.push_back(p);
		}
	}
	va_end(vl);
	return call_script(scriptid, outs, outi, keys, args);
} 

bool redisClient::setstr(const std::string& key, const std::string& val) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "SET %s %s", key.c_str(), val.c_str());
	CHECK_REPLY_RETBOOL(reply);
	freeReplyObject(reply);
	return true;
}

std::string redisClient::getstr(const std::string& key) {
	CHECK_CONTEXT_RETSTRING;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "GET %s", key.c_str());
	CHECK_REPLY_RETSTRING(reply);
	if (reply->type == REDIS_REPLY_NIL) {
		return "";
	}
	CHECK_REPLYTYPE_RETSTRING(reply, REDIS_REPLY_STRING);
	char* buf = NULL;
	GET_REPLY_STRING(reply, buf);
	freeReplyObject(reply);
	return buf;
}

bool redisClient::setint(const std::string& key, int val) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "SET %s %d", key.c_str(), val);
	CHECK_REPLY_RETBOOL(reply);
	freeReplyObject(reply);
	return true;
}

int redisClient::getint(const std::string& key) {
	CHECK_CONTEXT_RETINT;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "GET %s", key.c_str());
	CHECK_REPLY_RETINT(reply);
	if (reply->type == REDIS_REPLY_NIL) {
		return 0;
	}
	CHECK_REPLYTYPE_RETINT(reply, REDIS_REPLY_STRING);
	char* buf = NULL;
	GET_REPLY_STRING(reply, buf);
	int ret = atoi(buf);
	freeReplyObject(reply);
	return ret;
}

bool redisClient::del(const std::string& key) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "DEL %s", key.c_str());
	CHECK_REPLY_RETBOOL(reply);
	bool ret = (reply->type == REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return ret;
}

static redisReply* redisPush(bool l, redisContext* ctx, const std::string& key, const std::vector<std::string>& vals) {
	redisReply* reply = NULL;

	// lpush key vals...
	int argc = 1 + 1 + vals.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = l?"LPUSH":"RPUSH";
	char* pkey = (char*)alloca(key.size() + 1); std::copy(key.begin(), key.end(), pkey); pkey[key.size()] = 0;
	argv[idx++] = pkey;
	for (size_t i = 0; i < vals.size(); ++i) {
		char* val = (char*)alloca(vals[i].size() + 1); std::copy(vals[i].begin(), vals[i].end(), val); val[vals[i].size()] = 0;
		argv[idx++] = val;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::lpush(const std::string& key, const std::vector<std::string>& vals) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisPush(true, ctx_, key, vals);
	CHECK_REPLY_RETBOOL(reply);
	freeReplyObject(reply);
	return true;
}

bool redisClient::lpush(const std::string& key, int cnt, ...) {
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> vals;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		vals.push_back(p);
	}
	va_end(vl);
	redisReply* reply = (redisReply*)redisPush(true, ctx_, key, vals);
	CHECK_REPLY_RETBOOL(reply);
	freeReplyObject(reply);
	return true;
}

std::string redisClient::lpop(const std::string& key) {
	CHECK_CONTEXT_RETSTRING;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "LPOP %s", key.c_str());
	CHECK_REPLY_RETSTRING(reply);
	CHECK_REPLYTYPE_RETSTRING(reply, REDIS_REPLY_STRING);
	char* buf = NULL;
	GET_REPLY_STRING(reply, buf);
	freeReplyObject(reply);
	return buf;
}

bool redisClient::rpush(const std::string& key, const std::vector<std::string>& vals) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisPush(false, ctx_, key, vals);
	CHECK_REPLY_RETBOOL(reply);
	freeReplyObject(reply);
	return true;
}

bool redisClient::rpush(const std::string& key, int cnt, ...) {
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> vals;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		vals.push_back(p);
	}
	va_end(vl);
	redisReply* reply = (redisReply*)redisPush(false, ctx_, key, vals);
	CHECK_REPLY_RETBOOL(reply);
	freeReplyObject(reply);
	return true;
}

std::string redisClient::rpop(const std::string& key) {
	CHECK_CONTEXT_RETSTRING;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "RPOP %s", key.c_str());
	CHECK_REPLY_RETSTRING(reply);
	CHECK_REPLYTYPE_RETSTRING(reply, REDIS_REPLY_STRING);
	char* buf = NULL;
	GET_REPLY_STRING(reply, buf);
	freeReplyObject(reply);
	return buf;
}

int redisClient::llen(const std::string& key) {
	CHECK_CONTEXT_RETINT;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "LLEN %s", key.c_str());
	CHECK_REPLY_RETINT(reply);
	CHECK_REPLYTYPE_RETINT(reply, REDIS_REPLY_INTEGER);
	int ret = reply->integer;
	freeReplyObject(reply);
	return ret;
}

bool redisClient::lrem(const std::string& key, int count, const std::string& val) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "LREM %s %d %s", key.c_str(), count, val.c_str());
	CHECK_REPLY_RETBOOL(reply);
	bool ret = (reply->type == REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return ret;
}

bool redisClient::lrange(const std::string& key, int start, int stop, std::vector<std::string>& result) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "LRANGE %s %d %d", key.c_str(), start, stop);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; ++i) {
		redisReply* r = reply->element[i];
		if (r->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);
			result.push_back(buf);
		} else {
			printf("unexpected type: %d\n", r->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::lvisit(const std::string& key, int start, int stop, list_visit_callback cb) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "LRANGE %s %d %d", key.c_str(), start, stop);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; ++i) {
		redisReply* r = reply->element[i];
		if (r->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);
			cb(buf);
		} else {
			printf("unexpected type: %d\n", r->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::hgetall(const std::string& key, std::map<std::string, std::string>& result) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HGETALL %s", key.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; i+=2) {
		redisReply* rk = reply->element[i];
		redisReply* rv = reply->element[i+1];
		if (rk->type == REDIS_REPLY_STRING && rv->type == REDIS_REPLY_STRING) {
			char* bufkey = NULL;
			GET_REPLY_STRING(rk, bufkey);
			char* bufval = NULL;
			GET_REPLY_STRING(rv, bufval);
			result[bufkey] = bufval;
		} else {
			printf("unexpected type: %d, %d\n", rk->type, rv->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::hvisit(const std::string& key, hash_visit_callback cb) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HGETALL %s", key.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; i+=2) {
		redisReply* rk = reply->element[i];
		redisReply* rv = reply->element[i+1];
		if (rk->type == REDIS_REPLY_STRING && rv->type == REDIS_REPLY_STRING) {
			char* bufkey = NULL;
			GET_REPLY_STRING(rk, bufkey);
			char* bufval = NULL;
			GET_REPLY_STRING(rv, bufval);
			cb(bufkey, bufval);
		} else {
			printf("unexpected type: %d, %d\n", rk->type, rv->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

int redisClient::hlen(const std::string& key) {
	CHECK_CONTEXT_RETINT;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HLEN %s", key.c_str());
	CHECK_REPLY_RETINT(reply);
	CHECK_REPLYTYPE_RETINT(reply, REDIS_REPLY_INTEGER);
	int ret = reply->integer;
	freeReplyObject(reply);
	return ret;
}

bool redisClient::hkeys(const std::string& key, std::vector<std::string>& result) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HKEYS %s", key.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; ++i) {
		redisReply* r = reply->element[i];
		if (r->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);
			result.push_back(buf);
		} else {
			printf("unexpected type: %d\n", r->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::hexists(const std::string& key, const std::string& field) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HEXISTS %s %s", key.c_str(), field.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	int ret = reply->integer;
	freeReplyObject(reply);
	return ret > 0;
}

bool redisClient::hset(const std::string& key, const std::string& field, const std::string& val) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HSET %s %s %s", key.c_str(), field.c_str(), val.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

std::string redisClient::hget(const std::string& key, const std::string& field) {
	CHECK_CONTEXT_RETSTRING;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "HGET %s %s", key.c_str(), field.c_str());
	CHECK_REPLY_RETSTRING(reply);
	CHECK_REPLYTYPE_RETSTRING(reply, REDIS_REPLY_STRING);
	char* buf = NULL;
	GET_REPLY_STRING(reply, buf);
	freeReplyObject(reply);
	return buf;
}

static redisReply* redisHdel(redisContext* ctx, const std::string& key, const std::vector<std::string>& fields) {
	redisReply* reply = NULL;

	// hdel key fields...
	int argc = 1 + 1 + fields.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = "HDEL";
	char* pkey = (char*)alloca(key.size() + 1); std::copy(key.begin(), key.end(), pkey); pkey[key.size()] = 0;
	argv[idx++] = pkey;
	for (size_t i = 0; i < fields.size(); ++i) {
		char* f = (char*)alloca(fields[i].size() + 1); std::copy(fields[i].begin(), fields[i].end(), f); f[fields[i].size()] = 0;
		argv[idx++] = f;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::hdel(const std::string& key, const std::vector<std::string>& fields) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = redisHdel(ctx_, key, fields);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

bool redisClient::hdel(const std::string& key, int cnt, ...) {
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> fields;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		fields.push_back(p);
	}
	va_end(vl);
	redisReply* reply = redisHdel(ctx_, key, fields);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

static redisReply* redisSadd(redisContext* ctx, const std::string& key, const std::vector<std::string>& members) {
	redisReply* reply = NULL;

	// sadd key members...
	int argc = 1 + 1 + members.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = "SADD";
	char* pkey = (char*)alloca(key.size() + 1); std::copy(key.begin(), key.end(), pkey); pkey[key.size()] = 0;
	argv[idx++] = pkey;
	for (size_t i = 0; i < members.size(); ++i) {
		char* m = (char*)alloca(members[i].size() + 1); std::copy(members[i].begin(), members[i].end(), m); m[members[i].size()] = 0;
		argv[idx++] = m;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::sadd(const std::string& key, const std::vector<std::string>& members) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = redisSadd(ctx_, key, members);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

bool redisClient::sadd(const std::string& key, int cnt, ...) {
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> members;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		members.push_back(p);
	}
	va_end(vl);
	redisReply* reply = redisSadd(ctx_, key, members);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

static redisReply* redisSrem(redisContext* ctx, const std::string& key, const std::vector<std::string>& members) {
	redisReply* reply = NULL;

	// srem key members...
	int argc = 1 + 1 + members.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = "SREM";
	char* pkey = (char*)alloca(key.size() + 1); std::copy(key.begin(), key.end(), pkey); pkey[key.size()] = 0;
	argv[idx++] = pkey;
	for (size_t i = 0; i < members.size(); ++i) {
		char* m = (char*)alloca(members[i].size() + 1); std::copy(members[i].begin(), members[i].end(), m); m[members[i].size()] = 0;
		argv[idx++] = m;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::srem(const std::string& key, const std::vector<std::string>& members) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = redisSrem(ctx_, key, members);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

bool redisClient::srem(const std::string& key, int cnt, ...) {
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> members;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		members.push_back(p);
	}
	va_end(vl);
	redisReply* reply = redisSrem(ctx_, key, members);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

bool redisClient::sismember(const std::string& key, const std::string& member) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "SISMEMBER %s %s", key.c_str(), member.c_str());
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	int ret = reply->integer;
	freeReplyObject(reply);
	return ret > 0;
}

int redisClient::scard(const std::string& key) {
	CHECK_CONTEXT_RETINT;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "SCARD %s", key.c_str());
	CHECK_REPLY_RETINT(reply);
	CHECK_REPLYTYPE_RETINT(reply, REDIS_REPLY_INTEGER);
	int ret = reply->integer;
	freeReplyObject(reply);
	return ret;
}

static redisReply* redisZadd(redisContext* ctx, const std::string& key, const std::map<std::string, int>& pairs) {
	redisReply* reply = NULL;

	// zadd key score1 val1 score2 val2...
	int argc = 1 + 1 + 2 * pairs.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = "ZADD";
	char* pkey = (char*)alloca(key.size() + 1); std::copy(key.begin(), key.end(), pkey); pkey[key.size()] = 0;
	argv[idx++] = pkey;
	std::map<std::string, int>::const_iterator it = pairs.begin();
	for (; it != pairs.end(); ++it) {
		char* score = (char*)alloca(64);
		snprintf(score, 64, "%d", it->second);

		char* val = (char*)alloca(it->first.size() + 1);
		std::copy(it->first.begin(), it->first.end(), val);
		val[it->first.size()] = 0;

		argv[idx++] = score;
		argv[idx++] = val;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::zadd(const std::string& key, const std::map<std::string, int>& pairs) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = redisZadd(ctx_, key, pairs);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

bool redisClient::zadd(const std::string& key, int cnt, ...) { // cnt pairs of score and value
	CHECK_CONTEXT_RETBOOL;
	std::map<std::string, int> pairs;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		int s = va_arg(vl, int);
		const char* p = va_arg(vl, const char*);
		pairs[p] = s;
	}
	va_end(vl);
	redisReply* reply = redisZadd(ctx_, key, pairs);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

static redisReply* redisZrem(redisContext* ctx, const std::string& key, const std::vector<std::string>& members) {
	redisReply* reply = NULL;

	// zrem key members...
	int argc = 1 + 1 + members.size();
	const char** argv = (const char**)alloca(sizeof(char*) * argc); 
	int idx = 0;
	argv[idx++] = "ZREM";
	char* pkey = (char*)alloca(key.size() + 1); std::copy(key.begin(), key.end(), pkey); pkey[key.size()] = 0;
	argv[idx++] = pkey;
	for (size_t i = 0; i < members.size(); ++i) {
		char* m = (char*)alloca(members[i].size() + 1); std::copy(members[i].begin(), members[i].end(), m); m[members[i].size()] = 0;
		argv[idx++] = m;
	}
	reply = (redisReply*)redisCommandArgv(ctx, argc, argv, NULL);

	return reply;
}

bool redisClient::zrem(const std::string& key, const std::vector<std::string>& members) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = redisZrem(ctx_, key, members);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

bool redisClient::zrem(const std::string& key, int cnt, ...) { // cnt is number of members
	CHECK_CONTEXT_RETBOOL;
	std::vector<std::string> members;
	va_list vl;
	va_start(vl, cnt);
	for (int i = 0; i < cnt; ++i) {
		const char* p = va_arg(vl, const char*);
		members.push_back(p);
	}
	va_end(vl);
	redisReply* reply = redisZrem(ctx_, key, members);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_INTEGER);
	freeReplyObject(reply);
	return true;
}

int redisClient::zcard(const std::string& key) {
	CHECK_CONTEXT_RETINT;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "ZCARD %s", key.c_str());
	CHECK_REPLY_RETINT(reply);
	CHECK_REPLYTYPE_RETINT(reply, REDIS_REPLY_INTEGER);
	int ret = reply->integer;
	freeReplyObject(reply);
	return ret;
}

bool redisClient::zrange(const std::string& key, int start, int stop, std::vector<std::string>& result) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "ZRANGE %s %d %d", key.c_str(), start, stop);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; ++i) {
		redisReply* r = reply->element[i];
		if (r->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);
			result.push_back(buf);
		} else {
			printf("unexpected type: %d\n", r->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::zrange_withscores(const std::string& key, int start, int stop, std::map<std::string,int>& result) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "ZRANGE %s %d %d WITHSCORES", key.c_str(), start, stop);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; i+=2) {
		redisReply* r = reply->element[i];
		redisReply* s = reply->element[i+1];
		if (r->type == REDIS_REPLY_STRING && s->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);

			char* score = NULL;
			GET_REPLY_STRING(s, score);

			result[buf] = atoi(score);
		} else {
			printf("unexpected type: %d, %d\n", r->type, s->type);
		}
	}
	freeReplyObject(reply);
	return true;
}

bool redisClient::zvisit(const std::string& key, int start, int stop, zset_visit_callback cb) {
	CHECK_CONTEXT_RETBOOL;
	redisReply* reply = (redisReply*)redisCommand(ctx_, "ZRANGE %s %d %d WITHSCORES", key.c_str(), start, stop);
	CHECK_REPLY_RETBOOL(reply);
	CHECK_REPLYTYPE_RETBOOL(reply, REDIS_REPLY_ARRAY);
	for (size_t i=0; i<reply->elements; i+=2) {
		redisReply* r = reply->element[i];
		redisReply* s = reply->element[i+1];
		if (r->type == REDIS_REPLY_STRING && s->type == REDIS_REPLY_STRING) {
			char* buf = NULL;
			GET_REPLY_STRING(r, buf);

			char* score = NULL;
			GET_REPLY_STRING(s, score);

			cb(buf, atoi(score));
		} else {
			printf("unexpected type: %d, %d\n", r->type, s->type);
		}
	}
	freeReplyObject(reply);
	return true;
}
