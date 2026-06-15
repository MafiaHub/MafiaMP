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

    // A replicated vehicle. modelName carries the model used by the client to request the game car
    // (spawn-time identity, sent in the construction snapshot); VehicleSync::UpdateData carries the
    // per-tick physics/customization state. The server tracks which entity occupies each seat to
    // drive ownership and interest.
    class VehicleEntity : public Replication::NetworkEntity {
      public:
        static constexpr const char *kTypeName = "MafiaMP::Vehicle";
        static constexpr int kMaxSeats         = 4;

        std::string modelName;
        Modules::VehicleSync::UpdateData data {};

        // NetworkIDs of the human entity in each seat (0 = empty). Server-side bookkeeping; not part
        // of the replicated payload (seating is derived from each human's UpdateData.carPassenger).
        std::array<uint64_t, kMaxSeats> seats {};

        void OnSerializeConstruction(Replication::FieldSerializer &fields) override {
            MafiaNet::RakString name(modelName.c_str());
            fields.Field(name);
            if (!fields.Writing()) {
                modelName = name.C_String();
            }
        }

        void SerializeFields(Replication::FieldSerializer &fields) override {
            ForEachSyncedField([&](auto &field) {
                fields.Field(field);
            });
        }

        void SerializeForcedState(Replication::FieldSerializer &fields) override {
            ForEachConfigField([&](auto &field) {
                fields.Field(field);
            });
        }

      private:
        // Visits every replicated field, in a fixed order, so serialize and deserialize stay aligned.
        // VariableDeltaSerializer tracks each one independently — only changed fields go on the wire.
        template <typename Fn>
        void ForEachSyncedField(Fn &&fn) {
            fn(data.angularVelocity);
            fn(data.beaconLightsOn);
            fn(data.brake);
            fn(data.colorPrimary);
            fn(data.colorSecondary);
            fn(data.dirt);
            fn(data.engineOn);
            fn(data.fuel);
            fn(data.gear);
            fn(data.handbrake);
            fn(data.hornOn);
            fn(data.licensePlate);
            fn(data.lockState);
            fn(data.power);
            fn(data.radioOn);
            fn(data.radioStationId);
            fn(data.rimColor);
            fn(data.rust);
            fn(data.sirenOn);
            fn(data.steer);
            fn(data.tireColor);
            fn(data.velocity);
            fn(data.windowTint);
        }

        // Server-settable configuration, in a fixed order. Forced to the owner verbatim (no delta)
        // and otherwise replicated through ForEachSyncedField; keep both lists in sync.
        template <typename Fn>
        void ForEachConfigField(Fn &&fn) {
            fn(data.beaconLightsOn);
            fn(data.colorPrimary);
            fn(data.colorSecondary);
            fn(data.dirt);
            fn(data.engineOn);
            fn(data.fuel);
            fn(data.licensePlate);
            fn(data.lockState);
            fn(data.radioOn);
            fn(data.radioStationId);
            fn(data.rimColor);
            fn(data.rust);
            fn(data.sirenOn);
            fn(data.tireColor);
            fn(data.windowTint);
        }
    };
} // namespace MafiaMP::Shared::Entities
