#ifndef MESSAGE_ADAPTER_HEADER
#define MESSAGE_ADAPTER_HEADER

#include "message.h"
#include "luaport.h"

class inmessage_adapter : public inmessage
{
public:
    int command(lua_State* L) {
        lua_pushnumber(L, inmessage::command());
        return 1;
    }
    int read_int(lua_State* L) {
        lua_pushnumber(L, inmessage::read_int());
        return 1;
    }
    int read_int_and_repack(lua_State* L) {
        lua_pushnumber(L, inmessage::read_int_and_repack());
        return 1;
    }
    int read_short(lua_State* L) {
        lua_pushnumber(L, inmessage::read_short());
        return 1;
    }
    int read_char(lua_State* L) {
        lua_pushnumber(L, inmessage::read_char());
        return 1;
    }
    int read_cstring(lua_State* L) {
        lua_pushstring(L, inmessage::read_cstring().c_str());
        return 1;
    }
    int copy(lua_State* L) {
        void* buf = *(void**)lua_topointer(L, 2);
        int len = (int)lua_tonumber(L, 3);
        bool ret = inmessage::copy(buf, len);
        lua_pushnumber(L, ret);
        return 1;
    }
    static mfunc_item<inmessage_adapter> mfuncs[8];
};


class outmessage_adapter : public outmessage
{
public:
    int buffer(lua_State* L) {
        char* ret = outmessage::buffer();
        *(char**)lua_newuserdata(L,sizeof(char*)) = ret;
        return 1;
    }
    int size(lua_State* L) {
        lua_pushnumber(L, outmessage::size());
        return 1;
    }
    int write_int(lua_State* L) {
        int p1 = (int)lua_tonumber(L, 2);
        lua_pushnumber(L, outmessage::write_int(p1));
        return 1;
    }
    int write_char(lua_State* L) {
        char p1 = (char)lua_tonumber(L, 2);
        lua_pushnumber(L, outmessage::write_char(p1));
        return 1;
    }
    int write_short(lua_State* L) {
        short p1 = (short)lua_tonumber(L, 2);
        lua_pushnumber(L, outmessage::write_short(p1));
        return 1;
    }
    int write_front_int(lua_State* L) {
        int p1 = (int)lua_tonumber(L, 2);
        lua_pushnumber(L, outmessage::write_front_int(p1));
        return 1;
    }
    int write_front_char(lua_State* L) {
        char p1 = (char)lua_tonumber(L, 2);
        lua_pushnumber(L, outmessage::write_front_char(p1));
        return 1;
    }
    int write_cstring(lua_State* L) {
        const char* p1 = lua_tostring(L, 2);
        lua_pushnumber(L, outmessage::write_cstring(p1));
        return 1;
    }
    int begin(lua_State* L) {
        short cmd = (short)lua_tonumber(L, 2);
        outmessage::begin(cmd);
        return 0;
    }
    int end(lua_State* L) {
        outmessage::end();
        return 0;
    }
    static mfunc_item<outmessage_adapter> mfuncs[11];
};

typedef inmessage_adapter lua_inmessage;
typedef outmessage_adapter lua_outmessage;

#endif

