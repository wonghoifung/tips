#include "luaport.h"
#include "log.h"
#include "timer.h"
#include "SimpleServerAdapter.h"
#include "Peer.h"
#include <stdio.h>
#include <stdlib.h>

static int getpeerid_lua(lua_State* L) {
	tcpconn* conn = (tcpconn*)lua_topointer(L, -1);

	Peer* peer = getPeer(conn);

	lua_pushinteger(L, peer->getPeerId());
	return 1;
}

// test: call lua function, and pass a table as argument, called by sendtable_lua
static void call_printtable(lua_State* L) 
{
	printf("[%s:%d]---------> stack size: %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	lua_getglobal(lua_state(), "printtable");

	lua_newtable(L); // new table on top, roottable
	
	lua_pushnumber(L, 1);
	lua_pushstring(L, "abc"); 
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushinteger(L, 123);
	lua_settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushinteger(L, 456);
	lua_settable(L, -3);

	lua_pushnumber(L, 4);
	lua_pushinteger(L, 789);
	lua_settable(L, -3);

	lua_pushnumber(L, 5);
	lua_pushstring(L, "helloworld");
	lua_settable(L, -3);

	lua_call(lua_state(), 1, 0);

	printf("[%s:%d]---------> stack size: %d\n", __FUNCTION__, __LINE__, lua_gettop(L));
}

// test: called by lua, received a table argument
static int sendtable_lua(lua_State* L) 
{
	printf("[%s:%d]---------> stack size: %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	size_t len = lua_rawlen(L, -1);
	printf("len: %lu\n", len);
	for (size_t i = 0; i < len; ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -2);
		if (lua_isinteger(L, -1)) {
			printf("int: %d\n", (int)lua_tointeger(L, -1));
		} else if (lua_isstring(L, -1)) {
			printf("str: %s\n", lua_tostring(L, -1));
		} else {
			printf("err: dont support type other than int and str\n");
		}
		lua_pop(L, 1);
	}

	printf("[%s:%d]---------> stack size: %d\n", __FUNCTION__, __LINE__, lua_gettop(L));

	call_printtable(L);

	return 0;
}

// test: called by lua, install to lua as an table function, installed by testtable_lua
static int hello_lua(lua_State* L)
{
	printf("lua call c hello_lua\n");
	return 0;
}

// test: embedded table
static int testtable_lua(lua_State* L)
{
	printf("stack size = %d\n", lua_gettop(L));

	// receive table from lua
	lua_pushstring(L, "gdp");
	lua_gettable(L, 1); // pop key then push value
	if (lua_isinteger(L, -1)) {
		printf("int: %d\n", (int)lua_tointeger(L, -1));
	} else {
		printf("str: %s\n", lua_tostring(L, -1));
	}
	lua_pop(L, 1); // pop value
	printf("stack size = %d\n", lua_gettop(L));

	lua_pushstring(L, "info");
	lua_gettable(L, 1);
	printf("%s\n", lua_tostring(L, -1));
	lua_pop(L, 1);
	printf("stack size = %d\n", lua_gettop(L));

	lua_pushstring(L, "task"); // value is a table
	lua_gettable(L, 1);
	for (int i = 0; i < 4; ++i)
	{
		lua_pushnumber(L, i+1);
		lua_gettable(L, -2);
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	printf("stack size = %d\n", lua_gettop(L));
	
	// send table to lua
	lua_newtable(L); // new table on top, roottable
	lua_pushnumber(L, 1); // key
	lua_pushstring(L, "table2lua"); // value
	lua_settable(L, -3); // -3[-2] = -1

	lua_pushstring(L, "key"); 
	lua_newtable(L); // value is a table
	lua_pushstring(L, "hello");
	lua_pushcfunction(L, hello_lua); 
	lua_settable(L, -3); // newtable[hello] = hello_lua
	lua_pushnumber(L, 2);
	lua_pushnumber(L, 10086);
	lua_settable(L, -3); // newtable[2] = 10086
	lua_settable(L, -3); // roottable[key] = newtable
	printf("stack size = %d\n", lua_gettop(L));

	return 1;
}

int main() {
	if (!lua_init()) {
		printf("cannot init lua\n");
		exit(1);
	}
	
	luaport<SimpleServerAdapter>::register_class(lua_state(), "SimpleServer");
	lua_register(lua_state(), "getpeerid", getpeerid_lua);

	lua_register(lua_state(), "sendtable", sendtable_lua);
	lua_register(lua_state(), "testtable", testtable_lua);

	lua_dofile("./scripts/main.lua");
	
	lua_fini();
	
	exit(0);
}
