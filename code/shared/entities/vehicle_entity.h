#pragma once

#include <networking/replication/network_entity.h>

#include "shared/constants.h"
#include "shared/modules/vehicle_sync.hpp"

#include <mafianet/string.h>

#include <array>
#include <cstdint>
#include <string>

namespace MafiaMP::Shared::Entities {
    namespace Replication = Framework::Networking::Replication;

    // A replicated vehicle. The base modelName carries the model used by the client to request the
    // game car; VehicleSync::UpdateData carries the per-tick physics/customization state. The server
    // tracks which entity occupies each seat to drive ownership and interest.
    class VehicleEntity final : public Replication::NetworkEntity {
      public:
        static constexpr const char *kTypeName = "MafiaMP::Vehicle";
        static constexpr int kMaxSeats         = 4;

        Modules::VehicleSync::UpdateData data {};

        // NetworkIDs of the human entity in each seat (0 = empty). Server-side bookkeeping; not part
        // of the replicated payload (seating is derived from each human's UpdateData.carPassenger).
        std::array<uint64_t, kMaxSeats> seats {};

        void SerializeFields(MafiaNet::VariableDeltaSerializer *vds, MafiaNet::VariableDeltaSerializer::SerializationContext *ctx) override {
            vds->SerializeVariable(ctx, data);
        }

        void DeserializeFields(MafiaNet::VariableDeltaSerializer *vds, MafiaNet::VariableDeltaSerializer::DeserializationContext *ctx) override {
            vds->DeserializeVariable(ctx, data);
        }
    };
} // namespace MafiaMP::Shared::Entities
