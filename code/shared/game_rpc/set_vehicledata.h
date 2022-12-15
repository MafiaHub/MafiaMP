#pragma once

#include "src/networking/rpc/game_rpc.h"
#include "shared/modules/vehicle_sync.hpp"

#include <string>

namespace MafiaMP::Shared::RPC {
    class SetVehicleData final: public Framework::Networking::RPC::IGameRPC<SetVehicleData> {
      private:
        Shared::Modules::VehicleSync::UpdateData _data {};
      public:
        void FromParameters(const Shared::Modules::VehicleSync::UpdateData& data) {
            _data = data;
        }

        Shared::Modules::VehicleSync::UpdateData GetData() {
            return _data;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _data);
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
