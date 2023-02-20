#pragma once

#include <cstdint>

namespace MafiaMP::Game::Helpers { 
    namespace UI{
        enum MessageZone : uint8_t { HUD };

        enum MessageType : uint8_t { FREERIDE_BANNER };

        void SendMessageMovie(MessageZone, MessageType, char const *, char const *);
    };
} // namespace MafiaMP::Game::Helpers
