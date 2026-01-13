#pragma once

#include "src/networking/rpc/game_rpc.h"

namespace MafiaMP::Shared::RPC {
    class HumanChangeSkin final: public Framework::Networking::RPC::IGameRPC<HumanChangeSkin> {
      private:
        uint64_t _spawnProfile;
      public:
        HumanChangeSkin() = default;

        HumanChangeSkin(uint64_t spawnProfile)
            : _spawnProfile(spawnProfile) {}

        uint64_t GetSpawnProfile() const {
            return _spawnProfile;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _spawnProfile);
        }

        bool Valid() const override {
            return _spawnProfile > 0;
        }
    };
} // namespace MafiaMP::Shared::RPC
