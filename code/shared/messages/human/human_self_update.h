#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanSelfUpdate final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        uint64_t _spawnProfile {};

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_SELF_UPDATE;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _spawnProfile);
        }

        bool Valid() const override {
            return true;
        }

        void SetSpawnProfile(uint64_t profile) {
            _spawnProfile = profile;
        }

        uint64_t GetSpawnProfile() const {
            return _spawnProfile;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
