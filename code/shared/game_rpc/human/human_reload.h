#pragma once

#include <mafianet/BitStream.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // A weapon reload by the human identified by entityId, relayed to others.
    struct HumanReload {
        static constexpr const char *kIdentifier = "MafiaMP::HumanReload";

        uint64_t entityId = 0;
        int unk0          = 0;

        void FromParameters(uint64_t id, bool unk0_) {
            entityId = id;
            unk0     = unk0_;
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, entityId);
            bs->Serialize(write, unk0);
        }
    };
} // namespace MafiaMP::Shared::RPC
