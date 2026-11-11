#pragma once

#include <chrono>

#include "shared/entities/human_entity.h"

#include "sdk/entities/c_player_2.h"

#include "game/overrides/character_controller.h"
#include "game/streaming/entity_tracking_info.h"

#include <mafianet/replica_manager3.h>

#include <utils/snapshot_buffer.h>

#include <unordered_map>

#include <array>
#include <cstdint>
#include <deque>
#include <utility>

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
        // Remote humans: the poses received, sampled a short delay behind the newest one.
        Framework::Utils::TransformSnapshotBuffer poses {};
        // Remote humans: the owner's clock mapped onto ours.
        int64_t clockOffset          = 0;
        bool clockOffsetValid        = false;
        MafiaNet::Time clockOffsetAt = 0;
        // Remote humans: replicated states waiting for their pose to be shown, with their arrival.
        std::deque<std::pair<Shared::Modules::HumanSync::UpdateData, MafiaNet::Time>> pendingStates;
        static constexpr size_t kMaxPendingStates = 64;

        // Local enter/leave-vehicle state machine.
        CarEnterStates enterState = STATE_OUTSIDE;
        bool enterForced          = false;
        // When the last enter action was issued; a ped still not seated after kEnterCarTimeout is
        // warped into its seat.
        std::chrono::steady_clock::time_point enterIssuedAt {};
        static constexpr auto kEnterCarTimeout = std::chrono::seconds(2);

        bool isLocalPlayer = false;

        // Local player: where it was last frame, to send its velocity along with its pose.
        glm::vec3 lastLocalPosition {};
        MafiaNet::Time lastLocalTime = 0;

        // Last skin applied to the game ped, to detect replicated skin changes.
        uint64_t appliedSkin = 0;

        // --- Replica3 hooks ---
        void OnConstructed() override;
        void DeallocReplica(MafiaNet::Connection_RM3 *sourceConnection) override;
        void SerializeFields(Framework::Networking::Replication::FieldSerializer &fields) override;
        // Server overrode our state (teleport): move the game ped to the forced transform.
        void OnStateForced() override;
        // Remote humans: keep each pose the transform channel brings.
        void OnDeserialized(bool transformUpdated) override;

        // Per-frame driver for this human.
        void Frame();

        // Teleport the local player's game ped to this entity's replicated transform.
        void TeleportLocalToReplicated();

        // --- Module setup (called once from the client world) ---
        static void Install();
        static void UpdateAll();
        static Human *GetByPed(SDK::C_Human2 *ptr);
        static Human *GetByAgent(const SDK::ue::game::human::C_Agent *agent);

        // Remote humans out of a car keep the rotation replication gives them: the game would
        // turn them on its own, toward directions and with turn animations of its own.
        bool HoldsRotation() const;
        // Runs right after the game updated a remote ped's locomotion, which overwrote what we gave
        // it: puts back the owner's animation variables and the replicated rotation.
        void OnLocomotionUpdated();

      private:
        SDK::ue::sys::math::C_Quat heldRotation {};
        bool holdsRotation = false;
        // The owner's animation variables last applied, along with the pose shown.
        std::array<float, Shared::Modules::WANIM_VAR_SYNC_COUNT> animVariables {};

        void HoldRotation();
        void ApplyAnimVariables();
        void BindLocalPlayer();
        void RequestPed();
        void ApplyRemote();
        void ReadLocal();
        void ApplyPose();
        void DrawNametag();
    };
} // namespace MafiaMP::Core::Modules
