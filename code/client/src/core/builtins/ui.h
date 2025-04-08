#pragma once

#include <sol/sol.hpp>

#include <game/helpers/ui.h>

#include <sdk/mafia/ui/c_game_gui_2_module.h>

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

          static void FaderFadeIn(float duration, bool autoFadeOut) {
              const auto fader = SDK::mafia::ui::GetGameGui2Module()->GetFader();
              if (fader) {
                  SDK::ue::C_CntPtr<uintptr_t> syncObject;
                  fader->FadeIn(syncObject, duration, "CustomFaderFadeIn", autoFadeOut);
              }
          }

          static void FaderFadeOut(float duration, bool autoFadeIn) {
              const auto fader = SDK::mafia::ui::GetGameGui2Module()->GetFader();
              if (fader) {
                  SDK::ue::C_CntPtr<uintptr_t> syncObject;
                  fader->FadeOut(syncObject, duration, "CustomFaderFadeOut", autoFadeIn);
              }
          }

          static void FaderReset() {
              const auto fader = SDK::mafia::ui::GetGameGui2Module()->GetFader();
              if (fader) {
                  fader->Reset();
              }
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
              cls["faderFadeIn"]           = &UI::FaderFadeIn;
              cls["faderFadeOut"]          = &UI::FaderFadeOut;
              cls["faderReset"]            = &UI::FaderReset;
          }
    };
}
