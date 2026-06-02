#pragma once

#include "shared/entities/vehicle_entity.h"

#include "sdk/entities/c_car.h"

#include "game/streaming/entity_tracking_info.h"

#include <mafianet/ReplicaManager3.h>

#include <utils/interpolator.h>

#include <cstdint>

namespace SDK::ue::game::vehicle {
    class C_Vehicle;
} // namespace SDK::ue::game::vehicle

namespace MafiaMP::Core::Modules {
    // Client-side replicated vehicle: a VehicleEntity that owns and drives the local game car.
    // The owner (the driving client) reads the car back into the replicated state each frame; other
    // clients apply the replicated state to their car (interpolated).
    class Vehicle final : public Shared::Entities::VehicleEntity {
      public:
        SDK::C_Car *car                           = nullptr;
        Game::Streaming::EntityTrackingInfo *info = nullptr;
        Framework::Utils::Interpolator interpolator {};

        // --- Replica3 hooks ---
        void OnConstructed() override;
        void DeallocReplica(MafiaNet::Connection_RM3 *sourceConnection) override;
        void DeserializeFields(MafiaNet::VariableDeltaSerializer *vds, MafiaNet::VariableDeltaSerializer::DeserializationContext *ctx) override;
        // The server overrode our owned vehicle's configuration; apply it to the game car, then we
        // resume streaming it back from the corrected state.
        void OnStateForced() override;

        void Frame();
        bool IsOwnedByUs() const;
        // Apply the full replicated snapshot to the game car (remote vehicles, and once the streamed
        // car becomes ready).
        void ApplyRemote();

        static void Install();
        static void UpdateAll();
        static Vehicle *GetByCar(SDK::C_Car *carPtr);
        static Vehicle *GetByVehicle(SDK::ue::game::vehicle::C_Vehicle *vehiclePtr);
        static Vehicle *GetByNetworkId(uint64_t networkId);

      private:
        // Apply the server-settable configuration (engine, colors, fuel, ...) to the game car.
        void ApplyConfig();
        void ReadLocal();
    };
} // namespace MafiaMP::Core::Modules
