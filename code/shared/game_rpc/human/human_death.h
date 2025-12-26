#pragma once

#include "src/networking/rpc/game_rpc.h"

namespace MafiaMP::Shared::RPC {
    class HumanDeath final: public Framework::Networking::RPC::IGameRPC<HumanDeath> {
      public:
        HumanDeath() = default;

        void Serialize(SLNet::BitStream *bs, bool write) override {
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
