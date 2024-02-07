#pragma once

#include "shared/modules/vehicle_sync.hpp"
#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class VehiclePlayerLeave final: public Framework::Networking::RPC::IGameRPC<VehiclePlayerLeave> {
      public:
        uint64_t vehicleId;

        void FromParameters(VehiclePlayerLeave props) {
            this->vehicleId = props.vehicleId;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, vehicleId);
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
