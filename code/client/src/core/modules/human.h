#pragma once

#include "shared/entities/human_entity.h"

#include "sdk/entities/c_player_2.h"

#include "game/overrides/character_controller.h"
#include "game/streaming/entity_tracking_info.h"

#include <mafianet/ReplicaManager3.h>

#include <utils/interpolator.h>

#include <cstdint>

enum CarEnterStates {
    STATE_OUTSIDE,
    STATE_ENTERING,
    STATE_LEAVING,
    STATE_INSIDE
};

namespace MafiaMP::Core::Modules {
    // Client-side replicated human: a HumanEntity that also owns and drives the local game ped.
    // Remote humans apply the replicated state to their ped (interpolated); the local player reads
    // its ped back into the replicated state each frame so it serializes upstream.
    class Human final : public Shared::Entities::HumanEntity {
      public:
        SDK::C_Human2 *human                                 = nullptr;
        Game::Overrides::CharacterController *charController = nullptr;
        Game::Streaming::EntityTrackingInfo *info            = nullptr;
        Framework::Utils::Interpolator interpolator {};

        // Local enter/leave-vehicle state machine.
        CarEnterStates enterState = STATE_OUTSIDE;
        bool enterForced          = false;

        bool isLocalPlayer = false;

        // Last skin applied to the game ped, to detect replicated skin changes.
        uint64_t appliedSkin = 0;

        // --- Replica3 hooks ---
        void OnConstructed() override;
        void DeallocReplica(MafiaNet::Connection_RM3 *sourceConnection) override;
        // New network data arrived: apply remote state and refresh interpolation targets.
        void DeserializeFields(MafiaNet::VariableDeltaSerializer *vds, MafiaNet::VariableDeltaSerializer::DeserializationContext *ctx) override;

        // Per-frame driver for this human.
        void Frame();

        // --- Module setup (called once from the client world) ---
        static void Install();
        static void UpdateAll();
        static Human *GetByPed(SDK::C_Human2 *ptr);

      private:
        bool IsOwnedByUs() const;
        void BindLocalPlayer();
        void RequestPed();
        void ApplyRemote();
        void ReadLocal();
        void DrawNametag();
    };
} // namespace MafiaMP::Core::Modules
