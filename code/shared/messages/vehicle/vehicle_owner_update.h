#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

#include "shared/modules/vehicle_sync.hpp"

#include <glm/glm.hpp>

namespace MafiaMP::Shared::Messages::Vehicle {
    class VehicleOwnerUpdate final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        Modules::VehicleSync::UpdateData _updateData {};

      public:
        uint8_t GetMessageID() const override {
            return MOD_VEHICLE_OWNER_UPDATE;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _updateData);
        }

        bool Valid() const override {
            // todo
            return true;
        }

        Modules::VehicleSync::UpdateData GetData() const {
            return _updateData;
        }

        void SetData(const Modules::VehicleSync::UpdateData &data) {
            _updateData = data;
        }
    };
} // namespace MafiaMP::Shared::Messages::Vehicle
