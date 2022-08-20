#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

#include <string>

namespace MafiaMP::Shared::Messages::Vehicle {
    class VehicleSpawn final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        SLNet::RakString _modelName;

      public:
        uint8_t GetMessageID() const override {
            return MOD_VEHICLE_SPAWN;
        }

        void FromParameters(std::string modelName) {
            _modelName = modelName.c_str();
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _modelName);
        }

        bool Valid() const override {
            return true;
        }

        std::string GetModelName() const {
            return _modelName.C_String();
        }
    };
} // namespace MafiaMP::Shared::Messages::Vehicle
