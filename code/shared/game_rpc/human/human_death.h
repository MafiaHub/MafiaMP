#pragma once

#include "src/networking/rpc/game_rpc.h"

namespace MafiaMP::Shared::RPC {
    class HumanDeath final: public Framework::Networking::RPC::IGameRPC<HumanDeath> {
      public:
        void FromParameters(void) {
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
