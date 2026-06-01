#pragma once

#include <mafianet/BitStream.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // A player (resolved from the sender) entered the vehicle identified by vehicleId at seatIndex.
    struct VehiclePlayerEnter {
        static constexpr const char *kIdentifier = "MafiaMP::VehiclePlayerEnter";

        uint64_t vehicleId = 0;
        int seatIndex      = 0;

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, vehicleId);
            bs->Serialize(write, seatIndex);
        }
    };
} // namespace MafiaMP::Shared::RPC
