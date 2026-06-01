#pragma once

#include <mafianet/BitStream.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // Adds a weapon with ammo to the human identified by entityId.
    struct HumanAddWeapon {
        static constexpr const char *kIdentifier = "MafiaMP::HumanAddWeapon";

        uint64_t entityId = 0;
        int weaponId      = 0;
        int ammo          = 0;

        void FromParameters(uint64_t id, int weaponId_, int ammo_) {
            entityId = id;
            weaponId = weaponId_;
            ammo     = ammo_;
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, entityId);
            bs->Serialize(write, weaponId);
            bs->Serialize(write, ammo);
        }
    };
} // namespace MafiaMP::Shared::RPC
