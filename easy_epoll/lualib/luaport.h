#ifndef LUAPORT_HEADER
#define LUAPORT_HEADER

#include <lua.hpp>

template <typename T>
struct mfunc_item
{
    const char* name;
    int (T::*mfunc)(lua_State*);
};

template <typename T>
class luaport
{
public:
    static void register_class(lua_State* L, const char* cname)
    {
        lua_pushstring(L, cname);
        lua_pushcclosure(L, luaport::create_object, 1);
        lua_setglobal(L, cname);
        
        luaL_newmetatable(L, cname);
        lua_pushstring(L, "__gc");
        lua_pushstring(L, cname);
        lua_pushcclosure(L, luaport::destroy_object, 1);
        lua_settable(L, -3);
        
        for (unsigned i=0; T::mfuncs[i].name; ++i) {
            lua_pushstring(L, T::mfuncs[i].name);
            lua_pushnumber(L, i);
            lua_pushstring(L, cname);
            lua_pushcclosure(L, luaport::proxy, 2);
            lua_settable(L, -3);
        }
        
        lua_pushstring(L, "__index");
        lua_pushvalue(L, -2);
        lua_settable(L, -3);
    }
    
    static int create_object(lua_State* L)
    {
        const char* cname = lua_tostring(L, lua_upvalueindex(1));
        T* obj = new T();
        T** ud=(T**)lua_newuserdata(L,sizeof(T*));
        *ud = obj;
        luaL_getmetatable(L, cname);
        lua_setmetatable(L, -2);
        return 1;
    }
    
    static int destroy_object(lua_State* L)
    {
        const char* cname = lua_tostring(L, lua_upvalueindex(1));
        T** obj = (T**)luaL_checkudata(L, -1, cname);
        delete (*obj);
        return 0;
    }
    
    static int proxy(lua_State* L)
    {
        int i = (int)lua_tonumber(L, lua_upvalueindex(1));
        const char* cname = lua_tostring(L, lua_upvalueindex(2));
        T* obj = *(T**)luaL_checkudata(L, 1, cname);
        return (obj->*(T::mfuncs[i].mfunc))(L);
        return 0;
    }
};

lua_State* lua_state();
bool lua_init();
bool lua_fini();
void lua_dofile(const char* fname);

#endif
