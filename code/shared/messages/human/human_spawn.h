#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanSpawn final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        uint64_t _spawnProfile;

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_SPAWN;
        }

        void FromParameters(uint64_t spawnProfile) {
            _spawnProfile = spawnProfile;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _spawnProfile);
        }

        bool Valid() const override {
            return _spawnProfile > 0;
        }

        uint64_t GetSpawnProfile() const {
            return _spawnProfile;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
