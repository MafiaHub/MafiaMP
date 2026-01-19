#pragma once

#include "shared/modules/human_sync.hpp"
#include "src/networking/rpc/game_rpc.h"

#include <string>

namespace MafiaMP::Shared::RPC {
    class HumanAddWeapon final: public Framework::Networking::RPC::IGameRPC<HumanAddWeapon> {
      private:
        int weaponId;
        int ammo;

      public:
        HumanAddWeapon() = default;

        HumanAddWeapon(int _weaponId, int _ammo)
            : weaponId(_weaponId)
            , ammo(_ammo) {}

        int GetWeaponId() const {
            return weaponId;
        }

        int GetAmmo() const {
            return ammo;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, weaponId);
            bs->Serialize(write, ammo);
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
