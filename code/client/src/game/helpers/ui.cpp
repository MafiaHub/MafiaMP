#include "ui.h"

#include <fmt/format.h>

#include "../../core/application.h"

// These shit takes variadic arguments that we haven't reversed yet so w're forced to call it through lua
namespace MafiaMP::Game::Helpers::UI {
    void DisplayBannerMessage(char const *title, char const *content) {
        const auto command = fmt::format("game.hud:SendMessageMovie(\"HUD\", \"OnShowFreerideBanner\", \"{}\", \"{}\")", title, content);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void DisplayGenericMessage(char const *title, char const *content) {
        const auto command = fmt::format("game.hud:SendMessageMovie(\"HUD\", \"OnShowGenericMessage\", \"{}\", \"{}\")", title, content);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void DisplayTitleCard(char const *title, char const *content) {
        const auto command = fmt::format("game.hud:ShowTitleCard(\"{}\", \"{}\", true)", title, content);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void DisplayNote(char const* title, char const* content) {
        const auto command = fmt::format("game.hud:SendMessageMovie(\"HUD\", \"OnShowNote\", true, \"{}\", \"{}\")", title, content);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void HideTitleCard() {
        const auto command = fmt::format("game.hud:HideTitleCard()");
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void ToggleLoadSpinner(bool toggle) {
        const auto command = fmt::format("game.hud:ToggleSaveLoadSpinner({})", toggle);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void ShowNotification(char const *title, char const *content, int color) {
        const auto command = fmt::format("game.hud:SendMessageMovie(\"HUD\", \"OnShowFreerideNotification\", \"{}\", \"{}\", \"{}\")", title, content, color);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void HideNotification() {
        const auto command = fmt::format("game.hud:SendMessageMovie(\"HUD\", \"OnHideFreerideNotification\")");
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void DisplayMissionExit(char const *title, char const *content, int time) {
        const auto command = fmt::format("game.hud:ShowMissionExit(\"{}\", \"{}\", {})", title, content, time);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    void StartCountdown(int time) {
        const auto command = fmt::format("game.hud:StartCountDown({})", time);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }
}
