#pragma once

#include <string>

#define LOG_LUA "LuaVM"

namespace MafiaMP::Core {
    class LuaVM {
      private:
        std::string error;

      public:
        bool ExecuteString(const char *string);

        std::string GetError() const {
            return error;
        }
    };
} // namespace MafiaMP::Core
