#pragma once

#include "shared/modules/human_sync.hpp"
#include "src/networking/rpc/game_rpc.h"

#include <string>

namespace MafiaMP::Shared::RPC {
    class AddWeapon final: public Framework::Networking::RPC::IGameRPC<AddWeapon> {
      private:
        int weaponId;
        int ammo;

      public:
        void FromParameters(int _weaponId, int _ammo) {
            this->weaponId = _weaponId;
            this->ammo     = _ammo;
        }

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
