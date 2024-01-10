#pragma once

#include "shared/modules/vehicle_sync.hpp"
#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class VehicleSetProps final: public Framework::Networking::RPC::IGameRPC<VehicleSetProps> {
      public:
        Framework::Utils::Optional<bool> radioState;
        Framework::Utils::Optional<int> radioId;
        Framework::Utils::Optional<Modules::VehicleSync::LockState> locked;
        Framework::Utils::Optional<bool> beaconLightsState;
        Framework::Utils::Optional<bool> sirenState;
        Framework::Utils::Optional<SLNet::RakString> licensePlate;

        void FromParameters(VehicleSetProps props) {
            this->radioState        = props.radioState;
            this->radioId           = props.radioId;
            this->locked            = props.locked;
            this->beaconLightsState = props.beaconLightsState;
            this->sirenState        = props.sirenState;
            this->licensePlate      = props.licensePlate;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            radioState.Serialize(bs, write);
            radioId.Serialize(bs, write);
            locked.Serialize(bs, write);
            beaconLightsState.Serialize(bs, write);
            sirenState.Serialize(bs, write);
            licensePlate.Serialize(bs, write);
        }

        bool Valid() const override {
            if (radioId.HasValue() && (radioId() >= -1 && radioId() < 4 /* E_RADIO_POLICE */))
                return false;
            if (licensePlate.HasValue() && licensePlate().GetLength() > 7)
                return false;
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
