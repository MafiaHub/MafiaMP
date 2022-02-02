#include "luavm.h"
#include "../sdk/patterns.h"

#include <MinHook.h>
#include <logging/logger.h>
#include <utils/hooking/hooking.h>

#include <sstream>

#define LUA_MULTRET (-1)

using namespace SDK;

namespace MafiaMP::Core {
    typedef void lua_State;
    lua_State *g_luaState = nullptr;

    typedef int32_t(__cdecl *luaL_loadbuffer_t)(lua_State *L, const char *buff, size_t size, const char *name);
    luaL_loadbuffer_t pluaL_loadbuffer = nullptr;

    int32_t luaL_loadbuffer_(lua_State *L, const char *buff, size_t size, const char *name) {
        if (g_luaState == nullptr && L != nullptr) {
            Framework::Logging::GetLogger(LOG_LUA)->info("Lua wrapper is initialized.");
            g_luaState = L;
        }

        return pluaL_loadbuffer(L, "", 0, name);
    }

    typedef int32_t(__cdecl *lua_pcall_t)(lua_State *L, int32_t nargs, int32_t nresults, int32_t errfunc);
    lua_pcall_t plua_pcall = nullptr;

    int32_t lua_pcall_(lua_State *L, int32_t nargs, int32_t nresults, int32_t errfunc) {
        return plua_pcall(L, nargs, nresults, errfunc);
    }

    typedef const char *(__cdecl *lua_tostring_t)(lua_State *L, int32_t idx);
    lua_tostring_t plua_tostring = nullptr;

    const char *lua_tostring_(lua_State *L, int32_t idx) {
        return plua_tostring(L, idx);
    }

    typedef uint32_t(__cdecl *lua_isstring_t)(lua_State *L, int32_t idx);
    lua_isstring_t plua_isstring = nullptr;

    __declspec(dllexport) uint32_t lua_isstring_(lua_State *L, int32_t idx) {
        return plua_isstring(L, idx);
    }

    int luaL_loadstring(lua_State *L, const char *s) {
        return pluaL_loadbuffer(L, s, strlen(s), s);
    }

#ifdef luaL_dostring
#undef luaL_dostring
#endif // luaL_dostring

#define luaL_dostring(L, s) (luaL_loadstring(L, s) || plua_pcall(L, 0, LUA_MULTRET, 0))

    bool LuaVM::ExecuteString(const char *string) {
        if (g_luaState == nullptr || pluaL_loadbuffer == nullptr || plua_pcall == nullptr)
            return true;

        if (luaL_dostring(g_luaState, string)) {
            std::stringstream ss;
            ss << "Error loading Lua code into buffer:\n";
            ss << lua_tostring_(g_luaState, -1);
            Framework::Logging::GetLogger(LOG_LUA)->error(ss.str());
            error = ss.str();
            return false;
        }
        else if (lua_isstring_(g_luaState, -1)) {
            std::stringstream ss;
            ss << "Lua execute result:\n";
            ss << lua_tostring_(g_luaState, -1);
            Framework::Logging::GetLogger(LOG_LUA)->info(ss.str());
        }

        error = "";

        return true;
    }

    static InitFunction init([]() {
        MH_CreateHook(reinterpret_cast<void **>(gPatterns.Lua__pcallAddr), reinterpret_cast<void **>(lua_pcall_), reinterpret_cast<void **>(&plua_pcall));
        MH_CreateHook(reinterpret_cast<void **>(gPatterns.Lua__loadbufferAddr), reinterpret_cast<void **>(luaL_loadbuffer_), reinterpret_cast<void **>(&pluaL_loadbuffer));
        MH_CreateHook(reinterpret_cast<void **>(gPatterns.Lua__tostringAddr), reinterpret_cast<void **>(lua_tostring_), reinterpret_cast<void **>(&plua_tostring));
        MH_CreateHook(reinterpret_cast<void **>(gPatterns.Lua__isstringAddr), reinterpret_cast<void **>(lua_isstring_), reinterpret_cast<void **>(&plua_isstring));
    });
} // namespace MafiaMP::Core
