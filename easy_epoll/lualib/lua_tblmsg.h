#ifndef LUA_TBLMSG_HEADER
#define LUA_TBLMSG_HEADER

class inmessage;
class tcpconn;

void register_lua_tblmsg();
void call_lua_dispatch_tblmsg(inmessage* msg, tcpconn* conn);

#endif

