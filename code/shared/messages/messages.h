#pragma once

#include <networking/messages/messages.h>

namespace MafiaMP::Shared::Messages {
    enum ModMessages {
        // Human
        MOD_HUMAN_SPAWN = Framework::Networking::Messages::GameMessages::GAME_NEXT_MESSAGE_ID + 1,
        MOD_HUMAN_DESPAWN,
        MOD_HUMAN_UPDATE,
        MOD_HUMAN_SELF_UPDATE,
    };
} // namespace Framework::Networking::Messages
