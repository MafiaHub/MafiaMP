#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanSpawn final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        uint64_t _spawnProfile;
        SLNet::RakString _nickname;
        uint16_t _playerIndex;

        struct CarPassenger {
            uint64_t carId {};
            int seatId {};
        } _carPassenger {};

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_SPAWN;
        }

        void FromParameters(uint64_t spawnProfile, const std::string &nickname, uint16_t playerIndex) {
            _spawnProfile = spawnProfile;
            _nickname     = nickname.c_str();
            _playerIndex  = playerIndex;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _spawnProfile);
            bs->Serialize(write, _nickname);
            bs->Serialize(write, _playerIndex);
            bs->SerializeCompressed(write, _carPassenger);
        }

        bool Valid() const override {
            return _spawnProfile > 0;
        }

        uint64_t GetSpawnProfile() const {
            return _spawnProfile;
        }

        std::string GetNickname() const {
            return _nickname.C_String();
        }

        uint16_t GetPlayerIndex() const {
            return _playerIndex;
        }

        void SetCarPassenger(uint64_t carId, int seatId) {
            _carPassenger.carId  = carId;
            _carPassenger.seatId = seatId;
        }

        CarPassenger GetCarPassenger() const {
            return _carPassenger;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
