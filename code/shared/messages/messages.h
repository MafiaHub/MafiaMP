#pragma once

#include <networking/messages/messages.h>

namespace MafiaMP::Shared::Messages {
    enum ModMessages {
        // Human
        MOD_HUMAN_SPAWN = Framework::Networking::Messages::GameMessages::GAME_NEXT_MESSAGE_ID + 1,
    };
} // namespace Framework::Networking::Messages
