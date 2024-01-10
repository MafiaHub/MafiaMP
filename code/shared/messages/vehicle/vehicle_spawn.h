#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

#include <string>

namespace MafiaMP::Shared::Messages::Vehicle {
    class VehicleSpawn final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        SLNet::RakString _modelName;
        MafiaMP::Shared::Modules::VehicleSync::UpdateData _spawnData;

      public:
        uint8_t GetMessageID() const override {
            return MOD_VEHICLE_SPAWN;
        }

        void FromParameters(std::string modelName, MafiaMP::Shared::Modules::VehicleSync::UpdateData spawnData) {
            _modelName = modelName.c_str();
            _spawnData = spawnData;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _modelName);
            bs->Serialize(write, _spawnData);
        }

        bool Valid() const override {
            return true;
        }

        std::string GetModelName() const {
            return _modelName.C_String();
        }

        MafiaMP::Shared::Modules::VehicleSync::UpdateData GetSpawnData() {
            return _spawnData;
        };
    };
} // namespace MafiaMP::Shared::Messages::Vehicle
