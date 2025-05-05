#pragma once

#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

#include "shared/modules/human_sync.hpp"

namespace MafiaMP::Shared::RPC {
    class HumanSetProps final: public Framework::Networking::RPC::IGameRPC<HumanSetProps> {
      public:
        struct CarEnterExitAction {
            uint64_t carId {};
            int seatId {};
            bool forced {};
        };

        Framework::Utils::Optional<float> health {};
        Framework::Utils::Optional<CarEnterExitAction> carEnterExitAction {};

        void FromParameters(HumanSetProps props) {
            this->health = props.health;
            this->carEnterExitAction = props.carEnterExitAction;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            health.Serialize(bs, write);
            carEnterExitAction.Serialize(bs, write);
        }
        
        bool Valid() const override {
            if (health.HasValue() && !(health() >= 0.0f && health() <= 100.0f))
                return false;

            if (carEnterExitAction.HasValue() && !(carEnterExitAction().seatId >= 0 && carEnterExitAction().seatId <= 3))
                return false;

            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
