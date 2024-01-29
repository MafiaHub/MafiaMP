#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanSpawn final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        uint64_t _spawnProfile;

        struct CarPassenger {
            uint64_t carId{};
            int seatId{};
        } _carPassenger{};

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_SPAWN;
        }

        void FromParameters(uint64_t spawnProfile) {
            _spawnProfile = spawnProfile;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _spawnProfile);
            bs->SerializeCompressed(write, _carPassenger);
        }

        bool Valid() const override {
            return _spawnProfile > 0;
        }

        uint64_t GetSpawnProfile() const {
            return _spawnProfile;
        }

        void SetCarPassenger(uint64_t carId, int seatId) {
            _carPassenger.carId = carId;
            _carPassenger.seatId = seatId;
        }

        CarPassenger GetCarPassenger() const {
            return _carPassenger;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
