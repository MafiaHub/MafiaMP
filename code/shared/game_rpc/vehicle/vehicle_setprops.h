#pragma once

#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class VehicleSetProps final: public Framework::Networking::RPC::IGameRPC<VehicleSetProps> {
      public:
        Framework::Utils::Optional<int> radioId; // -1 == turn radio off

        void FromParameters(VehicleSetProps props) {
            this->radioId = props.radioId;
        }

        Framework::Utils::Optional<int> GetRadioID() {
            return radioId;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, radioId);
        }

        bool Valid() const override {
            if (radioId.HasValue() && (radioId() >= -1 && radioId() < 4 /* E_RADIO_POLICE */))
                return false;
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
