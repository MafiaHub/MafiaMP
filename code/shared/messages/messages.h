#pragma once

#include <networking/messages/messages.h>

namespace MafiaMP::Shared::Messages {
    enum ModMessages {
        // Human
        MOD_HUMAN_SPAWN = Framework::Networking::Messages::GameMessages::GAME_NEXT_MESSAGE_ID + 1,
        MOD_HUMAN_DESPAWN,
        MOD_HUMAN_UPDATE,
        MOD_HUMAN_SELF_UPDATE,

        // Vehicle
        MOD_VEHICLE_SPAWN,
        MOD_VEHICLE_DESPAWN,
        MOD_VEHICLE_UPDATE,
        MOD_VEHICLE_OWNER_UPDATE,
        MOD_VEHICLE_SELF_UPDATE,
    };
} // namespace Framework::Networking::Messages
