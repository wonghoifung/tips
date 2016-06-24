#include "lua_tblmsg.h"
#include "luaport.h"
#include "tcpconn.h"
#include "message.h"
#include "log.h"
#include <vector>
#include <map>
#include <assert.h>

typedef std::map<int, std::vector<int> > mapintvector;

static mapintvector tblmsg_formats;

/*
_M.tblmsg_format = {}
_M.tblmsg_format[_M.tblmsg_test1] = { 1, 2, 2, 2, 1 } -- 1 for int; 2 for str
*/
static int load_tblmsg_formats_lua(lua_State* L)
{
	lua_pushnil(L); // for next to start, -1:nil -2:table
	while (lua_next(L, -2)) {
		// -1:value -2:key -3:table
		int cmd = lua_tointeger(L, -2);

		// -1 is the sub table
		size_t len = lua_rawlen(L, -1);
		for (size_t i = 0; i < len; ++i) {
			lua_pushnumber(L, i+1);
			lua_gettable(L,-2);
			tblmsg_formats[cmd].push_back(lua_tointeger(L,-1));
			lua_pop(L, 1);
		}

		// pop the value(sub table)
		lua_pop(L, 1);
	}
	// no need to pop key, lua_next pop it for us, -1:table

	for (mapintvector::iterator it = tblmsg_formats.begin(); it != tblmsg_formats.end(); ++it) {
		printf("tblmsg %d: ", it->first);
		for (size_t i = 0; i < it->second.size(); ++i) {
			printf("%d ", it->second[i]);
		}
		printf("\n");
	}

	return 0;
}

static int sendtblmsg_lua(lua_State* L) 
{
	// lua call: sendtblmsg(conn, cmd, msg)
	//                       -3    -2   -1

	tcpconn* conn = (tcpconn*)lua_topointer(L, -3);
    if (conn == NULL) {
        printf("conn is null\n");
        return 0;
    }

	int cmd = lua_tointeger(L, -2);

	outmessage msg;
	msg.begin(cmd);

	size_t len = lua_rawlen(L, -1);
	for (size_t i = 0; i < len; ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -2);
		if (lua_isinteger(L, -1)) {
			msg.write_int((int)lua_tointeger(L, -1));
		} else if (lua_isstring(L, -1)) {
			msg.write_cstring(lua_tostring(L, -1));
		} else {
			printf("err: dont support %s\n", lua_typename(L, lua_type(L, -1)));
			lua_pop(L, 1);
			return 0;
		}
		lua_pop(L, 1);
	}

	
	msg.end();
	
	conn->sendmsg(&msg);

	return 0;
}

void register_lua_tblmsg()
{
	lua_register(lua_state(), "load_tblmsg_formats", load_tblmsg_formats_lua);
	lua_register(lua_state(), "sendtblmsg", sendtblmsg_lua);
}

// dispatch_tblmsg(cmd, tblmsg, conn)
void call_lua_dispatch_tblmsg(inmessage* msg, tcpconn* conn) 
{
	int cmd = msg->command();
	mapintvector::iterator it = tblmsg_formats.find(cmd);
	if (it == tblmsg_formats.end()) {
		printf("no format found for tblmsg cmd: %d\n", cmd);
		return;
	}

	lua_State* L = lua_state();

	lua_getglobal(lua_state(), "dispatch_tblmsg");

	lua_pushinteger(L, cmd);

	lua_newtable(L); // new table on top, roottable
	
	for (size_t i = 0; i < it->second.size(); ++i) {
		// 1 for int; 2 for str
		switch (it->second[i]) {
		case 1:
			lua_pushnumber(L, i + 1);
			lua_pushinteger(L, msg->read_int());
			lua_settable(L, -3);
			break;
		case 2:
			lua_pushnumber(L, i + 1);
			lua_pushstring(L, msg->read_cstring());
			lua_settable(L, -3);
			break;
		default:
			printf("error tblmsg field type: %d\n", it->second[i]);
			assert(0);
		}
	}

	lua_pushlightuserdata(L, (void*)conn);

	lua_call(lua_state(), 3, 0);
}
