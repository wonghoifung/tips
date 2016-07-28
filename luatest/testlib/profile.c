// from skynet lua-profile.c by cloudwu

#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>

#include <time.h>

#if defined(__APPLE__)
#include <mach/task.h>
#include <mach/mach.h>
#endif

#define NANOSEC 1000000000
#define MICROSEC 1000000

// #define DEBUG_LOG 1

static double
get_time() {
#if  !defined(__APPLE__)
	struct timespec ti;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ti);

	int sec = ti.tv_sec & 0xffff;
	int nsec = ti.tv_nsec;

	return (double)sec + (double)nsec / NANOSEC;	
#else
	struct task_thread_times_info aTaskInfo;
	mach_msg_type_number_t aTaskInfoCount = TASK_THREAD_TIMES_INFO_COUNT;
	if (KERN_SUCCESS != task_info(mach_task_self(), TASK_THREAD_TIMES_INFO, (task_info_t )&aTaskInfo, &aTaskInfoCount)) {
		return 0;
	}

	int sec = aTaskInfo.user_time.seconds & 0xffff;
	int msec = aTaskInfo.user_time.microseconds;

	return (double)sec + (double)msec / MICROSEC;
#endif
}

static inline double 
diff_time(double start) {
	double now = get_time();
	if (now < start) {
		return now + 0x10000 - start;
	} else {
		return now - start;
	}
}

static int
lstart(lua_State *L) {
	if (lua_type(L,1) == LUA_TTHREAD) {
		lua_settop(L,1); // 把放协程的位置变为栈顶
	} else {
		lua_pushthread(L); // 栈顶不是协程，就把协程压栈
	}
	lua_rawget(L, lua_upvalueindex(2)); // 第二个upvalue指向的index指向的table，pop出栈顶为key，table[key]压栈
	if (!lua_isnil(L, -1)) { // 不是nil表示已经设置过
		return luaL_error(L, "Thread %p start profile more than once", lua_topointer(L, 1));
	}
	lua_pushthread(L); // key
	lua_pushnumber(L, 0); // value
	lua_rawset(L, lua_upvalueindex(2)); // 第二个upvalue指向的index指向的table, table[key]=value，pop出key value

	lua_pushthread(L); // key
	double ti = get_time();
#ifdef DEBUG_LOG
	fprintf(stderr, "PROFILE [%p] start\n", L);
#endif
	lua_pushnumber(L, ti); // value
	lua_rawset(L, lua_upvalueindex(1)); // 第一个upvalue指向的index指向的table，用于设置starttime

	return 0;
}

static int
lstop(lua_State *L) {
	if (lua_type(L,1) == LUA_TTHREAD) {
		lua_settop(L,1);
	} else {
		lua_pushthread(L);
	}
	lua_rawget(L, lua_upvalueindex(1)); // 取出starttime
	if (lua_type(L, -1) != LUA_TNUMBER) {
		return luaL_error(L, "Call profile.start() before profile.stop()");
	} 
	double ti = diff_time(lua_tonumber(L, -1));
	lua_pushthread(L);
	lua_rawget(L, lua_upvalueindex(2)); // 取出totaltime
	double total_time = lua_tonumber(L, -1);

	lua_pushthread(L);
	lua_pushnil(L);
	lua_rawset(L, lua_upvalueindex(1)); // 重置starttime为nil

	lua_pushthread(L);
	lua_pushnil(L);
	lua_rawset(L, lua_upvalueindex(2)); // 重置totaltime为nil

	total_time += ti;
	lua_pushnumber(L, total_time); // 返回totaltime
#ifdef DEBUG_LOG
	fprintf(stderr, "PROFILE [%p] stop (%lf / %lf)\n", L, ti, total_time);
#endif

	return 1;
}

static int
timing_resume(lua_State *L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
#ifdef DEBUG_LOG
	lua_State *from = lua_tothread(L, -1);
#endif
	lua_rawget(L, lua_upvalueindex(2)); // 第二个upvalue指向的index指向的table，栈顶是一个协程co, 取table[co]是旧的total time
	if (lua_isnil(L, -1)) {		// check total time
		lua_pop(L,1); // 之前没有total time就不用加start time了
		// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	} else {
		lua_pop(L,1); // 删掉旧值
		lua_pushvalue(L,1); // 复制一个协程co到栈顶
		double ti = get_time();
#ifdef DEBUG_LOG
		fprintf(stderr, "PROFILE [%p] resume\n", from);
		// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
#endif
		lua_pushnumber(L, ti);
		lua_rawset(L, lua_upvalueindex(1));	// set start time 第一个upvalue指向的index指向的table，table[co]=starttime ti
	}

	lua_CFunction co_resume = lua_tocfunction(L, lua_upvalueindex(3)); // 第三个upvalue指向的index指向的函数指针

	return co_resume(L);
}

static int
lresume(lua_State *L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_pushvalue(L,1); // 把协程指针拷贝一份到栈顶
	
	return timing_resume(L);
}

static int
lresume_co(lua_State *L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	luaL_checktype(L, 2, LUA_TTHREAD);
	lua_rotate(L, 2, -1); // 把协程指针移到栈顶

	return timing_resume(L);
}

static int
timing_yield(lua_State *L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
#ifdef DEBUG_LOG
	lua_State *from = lua_tothread(L, -1);
#endif
	lua_rawget(L, lua_upvalueindex(2));	// check total time， 当前函数第二个upvalue指向的index，该index指向一个table，栈顶为key，key为一个协程，从lyield传入，取table[key]压栈
	if (lua_isnil(L, -1)) {
		lua_pop(L,1);
		// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	} else {
		double ti = lua_tonumber(L, -1); // total time
		lua_pop(L,1);

		lua_pushthread(L); // 协程压入
		lua_rawget(L, lua_upvalueindex(1)); // 当前函数的第一个upvalue指向的index，该index指向的table，以上面的协程为索引，取得starttime 
		double starttime = lua_tonumber(L, -1); // start time
		lua_pop(L,1);

		double diff = diff_time(starttime); // now - start time
		ti += diff; // 更新时间开销
#ifdef DEBUG_LOG
		fprintf(stderr, "PROFILE [%p] yield (%lf/%lf)\n", from, diff, ti);
		// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
#endif

		lua_pushthread(L); // 协程入栈作为key
		lua_pushnumber(L, ti); // 更新后的ti入栈作为value
		lua_rawset(L, lua_upvalueindex(2)); // 更新当前函数第二个upvalue指向的index指向的table，t[key]=value
	}

	lua_CFunction co_yield = lua_tocfunction(L, lua_upvalueindex(3)); // lua_CFuntion是一个c函数指针类型，面向lua调用的一个接口，取出当前函数的第三个upvalue指向的index指向的函数指针

	return co_yield(L); // 执行这个函数
}

static int
lyield(lua_State *L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	lua_pushthread(L);

	return timing_yield(L);
}

static int
lyield_co(lua_State *L) {
	// printf("%s(%d): %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
	luaL_checktype(L, 1, LUA_TTHREAD); // 检查类型, 不是协程会挂，不会增加stack
	lua_rotate(L, 1, -1); // 把1指向的协程放到栈顶
	
	return timing_yield(L);
}

int
luaopen_testlib_profile(lua_State *L) {
	// #ifdef DEBUG_LOG
	// printf("%s\n", __FUNCTION__);
	// #endif
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "start", lstart },
		{ "stop", lstop },
		{ "resume", lresume },
		{ "yield", lyield },
		{ "resume_co", lresume_co },
		{ "yield_co", lyield_co },
		{ NULL, NULL },
	};
	luaL_newlibtable(L,l);
	lua_newtable(L);	// table thread->start time
	lua_newtable(L);	// table thread->total time

	lua_newtable(L);	// weak table
	lua_pushliteral(L, "kv"); // 同lua_pushstring，复制一个string
	lua_setfield(L, -2, "__mode"); // -2指向的weak table，key:__mode, value：栈顶, weak_table[__mode] = kv, pop出value

	lua_pushvalue(L, -1); // 复制一个weak table引用在栈顶
	lua_setmetatable(L, -3); // pop出上面复制的weak table，设置为total time table的metatable
	lua_setmetatable(L, -3); // pop出第一个weak table，设置为start time table的metatable

	lua_pushnil(L);	// cfunction (coroutine.resume or coroutine.yield)， 留出位置给下面设置lua自己的协程函数
	luaL_setfuncs(L,l,3); // 所有函数共享3个upvalues，这里调用完毕会pop出上面三个value

	int libtable = lua_gettop(L); // libtable的index栈位置

	lua_getglobal(L, "coroutine");
	lua_getfield(L, -1, "resume"); // coroutine table里的resume key对应的value压顶

	lua_CFunction co_resume = lua_tocfunction(L, -1); // 取出lua自己的resume的c函数
	if (co_resume == NULL)
		return luaL_error(L, "Can't get coroutine.resume");
	lua_pop(L,1);

	lua_getfield(L, libtable, "resume"); // libtable里的resume key对应的函数压顶
	lua_pushcfunction(L, co_resume); // lua自己的resume函数压顶
	lua_setupvalue(L, -2, 3); // -2指向的闭包，该闭包的第3个upvalue，设置为栈顶的lua自己的resume函数，pop出栈顶
	lua_pop(L,1); // pop出resume闭包

	lua_getfield(L, libtable, "resume_co");
	lua_pushcfunction(L, co_resume);
	lua_setupvalue(L, -2, 3);
	lua_pop(L,1);

	lua_getfield(L, -1, "yield");

	lua_CFunction co_yield = lua_tocfunction(L, -1); // 取出lua自己的yield c函数
	if (co_yield == NULL)
		return luaL_error(L, "Can't get coroutine.yield");
	lua_pop(L,1);

	lua_getfield(L, libtable, "yield");
	lua_pushcfunction(L, co_yield);
	lua_setupvalue(L, -2, 3);
	lua_pop(L,1);

	lua_getfield(L, libtable, "yield_co");
	lua_pushcfunction(L, co_yield);
	lua_setupvalue(L, -2, 3);
	lua_pop(L,1);

	lua_settop(L, libtable);

	return 1;
}
