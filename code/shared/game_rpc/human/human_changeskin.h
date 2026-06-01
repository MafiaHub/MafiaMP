#pragma once

#include <mafianet/BitStream.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // Changes the skin (spawn profile) of the human identified by entityId.
    struct HumanChangeSkin {
        static constexpr const char *kIdentifier = "MafiaMP::HumanChangeSkin";

        uint64_t entityId     = 0;
        uint64_t spawnProfile = 0;

        void FromParameters(uint64_t id, uint64_t profile) {
            entityId     = id;
            spawnProfile = profile;
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, entityId);
            bs->Serialize(write, spawnProfile);
        }

        bool Valid() const {
            return spawnProfile > 0;
        }
    };
} // namespace MafiaMP::Shared::RPC
