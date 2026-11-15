#pragma once

#include <chrono>

#include "shared/entities/human_entity.h"

#include "sdk/entities/c_player_2.h"

#include "game/overrides/character_controller.h"
#include "game/streaming/entity_tracking_info.h"

#include <mafianet/replica_manager3.h>

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
        bool leaveForced          = false;
        // When the last enter, leave or seat-switch action was issued. A ped still not seated
        // kCarActionTimeout after an enter is warped into its seat; one still in its car that long
        // after a leave has the leave retried, forced.
        std::chrono::steady_clock::time_point carActionIssuedAt {};
        static constexpr auto kCarActionTimeout = std::chrono::seconds(2);

        bool isLocalPlayer = false;

        // Remote humans: the spawn profile `info` was requested for, or the last one that failed to
        // spawn, so a failure is not retried every frame.
        uint64_t appliedSkin = 0;
        // Remote humans: a ped cannot change spawn profile in place, so one of the new profile is
        // requested beside the current one and replaces it once spawned.
        Game::Streaming::EntityTrackingInfo *reskinInfo = nullptr;
        uint64_t reskinSkin                             = 0;

        // --- Replica3 hooks ---
        void OnConstructed() override;
        void DeallocReplica(MafiaNet::Connection_RM3 *sourceConnection) override;
        void SerializeFields(Framework::Networking::Replication::FieldSerializer &fields) override;
        // Server overrode our state (teleport): move the game ped to the forced transform.
        void OnStateForced() override;

        // Per-frame driver for this human.
        void Frame();

        // Local player: switch the game's player model to modelHash when it changed.
        void ApplyLocalSkin();

        // Remote humans: request a ped of modelHash when it changed. Not called from Frame(), which
        // also runs from the entity factory's own request callback.
        void UpdateRemoteSkin();

        // Teleport the local player's game ped to this entity's replicated transform.
        void TeleportLocalToReplicated();

        // --- Module setup (called once from the client world) ---
        static void Install();
        static void UpdateAll();
        static Human *GetByPed(SDK::C_Human2 *ptr);

      private:
        void BindLocalPlayer();
        Game::Streaming::EntityTrackingInfo *RequestPed();
        void ApplyRemote();
        bool ApplyRemoteCarExit();
        void ApplyRemoteSeatSwitch();
        void ReadLocal();
        void ReadLocalCarSeat();
        void DrawNametag();
    };
} // namespace MafiaMP::Core::Modules
