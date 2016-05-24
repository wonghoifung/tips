#include "lua_timer.h"
#include "timer.h"
#include "luaport.h"
#include <map>

struct lua_timer {
	int tid;
	timer t;
	char cbname[256];
};

static int timerserial = 0;

static std::map<int, lua_timer*> gtimers;

class lua_timer_handler : public timer_handler
{
public:
    lua_timer_handler() {}
    virtual ~lua_timer_handler() {}
	virtual int on_timeout(int tid) {
		lua_timer* ltimer = gtimers[tid];

		if (!ltimer) {
			printf("timer %d no callback\n", tid);
			return -1;
		}

		printf("%s\n", ltimer->cbname);

		lua_getglobal(lua_state(), ltimer->cbname);

	    lua_pushlightuserdata(lua_state(), (void*)ltimer);

	    lua_call(lua_state(), 1, 0);

	    return 0;
	}
};

static lua_timer_handler glth;

static int create_timer_lua(lua_State* L) {
	const char* callbackname = lua_tostring(L, -1);

	lua_timer* t = new lua_timer();
	int tid = ++timerserial;
	t->tid = tid;
	t->t.set_timerid(tid);
	snprintf(t->cbname, sizeof t->cbname, callbackname);
	t->t.set_handler(&glth, tid);

	gtimers[tid] = t;

	lua_pushlightuserdata(L, (void*)t);
	return 1;
}

static int destroy_timer_lua(lua_State* L) {
	lua_timer* t = (lua_timer*)lua_topointer(L, -1); // todo need to pop lua stack?
	gtimers.erase(t->tid);
	delete t;
	return 0;
}

static int start_timer_lua(lua_State* L) {
	lua_timer* t = (lua_timer*)lua_topointer(L, -1); 
	int sec = (int)lua_tointeger(L, -2);
	t->t.start(sec);
	return 0;
}

static int stop_timer_lua(lua_State* L) {
	lua_timer* t = (lua_timer*)lua_topointer(L, -1); 
	t->t.stop();
	return 0;
}

void register_lua_timer() {
	lua_register(lua_state(), "create_timer", create_timer_lua);
	lua_register(lua_state(), "destroy_timer", destroy_timer_lua);
	lua_register(lua_state(), "start_timer", start_timer_lua);
	lua_register(lua_state(), "stop_timer", stop_timer_lua);
}
