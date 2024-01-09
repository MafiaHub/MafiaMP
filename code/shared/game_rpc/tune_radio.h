#pragma once

#include "src/networking/rpc/game_rpc.h"

namespace MafiaMP::Shared::RPC {
    class TuneRadio final: public Framework::Networking::RPC::IGameRPC<TuneRadio> {
      private:
        int _radioId; // -1 == turn radio off

      public:
        void FromParameters(int radioId) {
            _radioId = radioId;
        }

        int GetRadioID() {
            return _radioId;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _radioId);
        }

        bool Valid() const override {
            return _radioId >= -1 && _radioId < 4 /* E_RADIO_POLICE */;
        }
    };
} // namespace MafiaMP::Shared::RPC
