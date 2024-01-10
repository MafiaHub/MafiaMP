#pragma once

#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class HumanSetProps final: public Framework::Networking::RPC::IGameRPC<HumanSetProps> {
      public:
        Framework::Utils::Optional<float> health {};

        void FromParameters(HumanSetProps props) {
            this->health = props.health;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            health.Serialize(bs, write);
        }
        
        bool Valid() const override {
            return (health.HasValue() && health() >= 0.0f && health() <= 100.0f);
        }
    };
} // namespace MafiaMP::Shared::RPC
