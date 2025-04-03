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

          static void DisplayTitleCard(const char *title, const char *message) {
              if (!title || !message) {
                  return;
              }
              MafiaMP::Game::Helpers::UI::DisplayTitleCard(title, message);
          }

          static void HideTitleCard() {
              MafiaMP::Game::Helpers::UI::HideTitleCard();
          }

          static void DisplayNote(const char *title, const char *message) {
              if (!title || !message) {
                  return;
              }
              MafiaMP::Game::Helpers::UI::DisplayNote(title, message);
          }

          static void ToggleSaveLoadSpinner(bool toggle) {
              MafiaMP::Game::Helpers::UI::ToggleLoadSpinner(toggle);
          }

          static void ShowNotification(const char *title, const char *message, int color) {
              if (!title || !message) {
                  return;
              }
              MafiaMP::Game::Helpers::UI::ShowNotification(title, message, color);
          }

          static void HideNotification() {
              MafiaMP::Game::Helpers::UI::HideNotification();
          }

          static void DisplayMissionExit(const char *title, const char *message, int time) {
              if (!title || !message) {
                  return;
              }
              MafiaMP::Game::Helpers::UI::DisplayMissionExit(title, message, time);
          }

          static void StartCountdown(int time) {
              MafiaMP::Game::Helpers::UI::StartCountdown(time);
          }

          static void Register(sol::state* luaEngine) {
              if (!luaEngine) {
                  return;
              }

              sol::usertype<UI> cls = luaEngine->new_usertype<UI>("UI");
              cls["displayBannerMessage"] = &UI::DisplayBannerMessage;
              cls["displayGenericMessage"] = &UI::DisplayGenericMessage;
              cls["displayTitleCard"]      = &UI::DisplayTitleCard;
              cls["hideTitleCard"]         = &UI::HideTitleCard;
              cls["displayNote"]           = &UI::DisplayNote;
              cls["toggleSaveLoadSpinner"] = &UI::ToggleSaveLoadSpinner;
              cls["showNotification"]      = &UI::ShowNotification;
              cls["hideNotification"]      = &UI::HideNotification;
              cls["displayMissionExit"]    = &UI::DisplayMissionExit;
              cls["startCountdown"]        = &UI::StartCountdown;
          }
    };
}
