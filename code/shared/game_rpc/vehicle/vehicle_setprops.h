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
        Framework::Utils::Optional<bool> beaconLights;
        Framework::Utils::Optional<bool> siren;
        Framework::Utils::Optional<SLNet::RakString> licensePlate;

        void FromParameters(VehicleSetProps props) {
            this->radioState   = props.radioState;
            this->radioId      = props.radioId;
            this->locked       = props.locked;
            this->beaconLights = props.beaconLights;
            this->siren        = props.siren;
            this->licensePlate = props.licensePlate;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            radioState.Serialize(bs, write);
            radioId.Serialize(bs, write);
            locked.Serialize(bs, write);
            beaconLights.Serialize(bs, write);
            siren.Serialize(bs, write);
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
