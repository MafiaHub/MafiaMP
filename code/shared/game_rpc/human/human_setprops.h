#pragma once

#include <mafianet/BitStream.h>
#include <utils/optional.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // Sets properties (currently health) on the human identified by entityId.
    struct HumanSetProps {
        static constexpr const char *kIdentifier = "MafiaMP::HumanSetProps";

        uint64_t entityId = 0;
        Framework::Utils::Optional<float> health {};

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, entityId);
            health.Serialize(bs, write);
        }

        bool Valid() const {
            return health.HasValue() && health() >= 0.0f && health() <= 100.0f;
        }
    };
} // namespace MafiaMP::Shared::RPC
