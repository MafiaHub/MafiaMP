
#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanDespawn final: public Framework::Networking::Messages::GameSyncMessage {
      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_DESPAWN;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            // noop
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
