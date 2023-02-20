#include "ui.h"

#include <fmt/format.h>

#include "../../core/application.h"

namespace MafiaMP::Game::Helpers::UI {
    const char *GetMessageZoneStr(MessageZone z) {
        switch (z) {
        case HUD: return "HUD";
        default: return "UNKNOWN";
        }
    }

    const char *GetMessageTypeStr(MessageType t) {
        switch (t) {
        case FREERIDE_BANNER: return "OnShowFreerideBanner";
        default: return "UNKNOWN";
        }
    }

    // This shit takes variadic arguments that we haven't reversed yet so w're forced to call it through lua
    void SendMessageMovie(MessageZone zone, MessageType type, char const *title, char const *content) {
        const auto zoneStr = GetMessageZoneStr(zone);
        const auto typeStr = GetMessageTypeStr(type);
        const auto command = fmt::format("game.hud:SendMessageMovie(\"{}\", \"{}\", \"{}\", \"{}\")", zoneStr, typeStr, title, content);
        Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }
}
