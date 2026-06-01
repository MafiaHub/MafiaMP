#pragma once

#include <mafianet/BitStream.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // A player (resolved from the sender) left the vehicle identified by vehicleId.
    struct VehiclePlayerLeave {
        static constexpr const char *kIdentifier = "MafiaMP::VehiclePlayerLeave";

        uint64_t vehicleId = 0;

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, vehicleId);
        }
    };
} // namespace MafiaMP::Shared::RPC
