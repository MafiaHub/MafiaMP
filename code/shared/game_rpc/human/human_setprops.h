#pragma once

#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class HumanSetProps final: public Framework::Networking::RPC::IGameRPC<HumanSetProps> {
      private:
        Framework::Utils::Optional<float> health{};
      public:
        void FromParameters(Framework::Utils::Optional<float> _health) {
            this->health = _health;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            health.Serialize(bs, write);
        }

        Framework::Utils::Optional<float> GetHealth() const {
            return health;
        }

        bool Valid() const override {
            return (health.HasValue() && health() >= 0.0f && health() <= 100.0f);
        }
    };
} // namespace MafiaMP::Shared::RPC
