#pragma once

#include "shared/modules/vehicle_sync.hpp"
#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class VehiclePlayerEnter final : public Framework::Networking::RPC::IGameRPC<VehiclePlayerEnter> {
      public:
        uint64_t vehicleId;
        int seatIndex;

        void FromParameters(VehiclePlayerEnter props) {
            this->vehicleId = props.vehicleId;
            this->seatIndex = props.seatIndex;
        }

        void Serialize(SLNet::BitStream* bs, bool write) override {
            bs->Serialize(write, vehicleId);
            bs->Serialize(write, seatIndex);
        }

        bool Valid() const override {
            // TODO: validate seat index
            return true;
        }
    };
}
