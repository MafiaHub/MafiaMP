#pragma once

#include <mafianet/BitStream.h>

namespace MafiaMP::Shared::RPC {
    // The sending client reports its player died; the server resolves the player from the sender.
    struct HumanDeath {
        static constexpr const char *kIdentifier = "MafiaMP::HumanDeath";

        void FromParameters() {}

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            (void)bs;
            (void)write;
        }
    };
} // namespace MafiaMP::Shared::RPC
