#pragma once

#include "src/networking/rpc/game_rpc.h"

namespace MafiaMP::Shared::RPC {
    class HumanReload final: public Framework::Networking::RPC::IGameRPC<HumanReload> {
      private:
        int _unk0;
      public:
        void FromParameters(bool unk0) {
            _unk0 = unk0;
        }

        bool GetUnk0() const {
            return _unk0;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _unk0);
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
