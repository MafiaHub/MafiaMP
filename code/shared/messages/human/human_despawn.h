
#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanDespawn final: public Framework::Networking::Messages::GameSyncMessage {
      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_DESPAWN;
        }

        void FromParameters(flecs::entity_t serverID) {
            _serverID = serverID;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _serverID);
        }

        bool Valid() override {
            return ValidServerID();
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
