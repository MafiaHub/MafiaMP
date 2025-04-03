#pragma once

#include <sol/sol.hpp>

#include <game/helpers/ui.h>

namespace MafiaMP::Scripting {
    class UI final {
      public:
          static void DisplayBannerMessage(const char* title, const char* message) {
              if (!title || !message) {
                return;
              }
              MafiaMP::Game::Helpers::UI::DisplayBannerMessage(title, message);
          }

          static void DisplayGenericMessage(const char *title, const char *message) {
              if (!title || !message) {
                  return;
              }
              MafiaMP::Game::Helpers::UI::DisplayGenericMessage(title, message);
          }

          static void Register(sol::state* luaEngine) {
              if (!luaEngine) {
                  return;
              }

              sol::usertype<UI> cls = luaEngine->new_usertype<UI>("UI");
              cls["displayBannerMessage"] = &UI::DisplayBannerMessage;
              cls["displayGenericMessage"] = &UI::DisplayGenericMessage;
          }
    };
}
