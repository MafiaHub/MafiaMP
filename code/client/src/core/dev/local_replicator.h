#pragma once

#include <utils/safe_win32.h>

#include <utils/interpolator.h>

#include "game/overrides/character_controller.h"
#include "game/streaming/entity_tracking_info.h"
#include "sdk/entities/c_human_2.h"
#include "shared/modules/human_sync.hpp"

namespace MafiaMP::Core::Dev {
    /**
     * Local replication system for testing ped synchronization without networking.
     * Mirrors local player state to a spawned temp human using the same logic
     * as the network replication system (Human::Update, UpdateRemoteHuman).
     */
    class LocalReplicator final {
      public:
        LocalReplicator()  = default;
        ~LocalReplicator() = default;

        // Spawns a temp human and begins replication
        void SpawnAndStartReplication(uint64_t spawnProfile);

        // Stops replication and removes the temp human
        void StopAndDespawn();

        // Updates the replication - call this each frame
        void Update();

        // Returns true if the replicator is active
        bool IsActive() const {
            return _trackingInfo != nullptr && _human != nullptr;
        }

        // Get the replicated human
        SDK::C_Human2 *GetHuman() const {
            return _human;
        }

        // Toggle aim on the replicated human (for testing)
        void ToggleAim();

        // Set offset from player position
        void SetPositionOffset(const glm::vec3 &offset) {
            _positionOffset = offset;
        }

        const glm::vec3 &GetPositionOffset() const {
            return _positionOffset;
        }

      private:
        // Collects state from the local player (same as UpdateLocalPlayer system)
        void CollectLocalPlayerState();

        // Applies the collected state to the temp human (same as Human::Update)
        void ApplyStateToHuman();

        // Update interpolation (same as UpdateRemoteHuman system)
        void UpdateInterpolation();

        Game::Streaming::EntityTrackingInfo *_trackingInfo    = nullptr;
        SDK::C_Human2 *_human                                 = nullptr;
        Game::Overrides::CharacterController *_charController = nullptr;

        // Sync data - same structure as network replication
        Shared::Modules::HumanSync::UpdateData _syncData = {};

        // Interpolator for smooth position/rotation blending
        Framework::Utils::Interpolator _interpolator = {};

        // Position offset from local player (so they don't overlap)
        glm::vec3 _positionOffset = {2.0f, 0.0f, 0.0f};

        // Transform target from sync
        glm::vec3 _targetPos = {};
        glm::quat _targetRot = {};
    };
} // namespace MafiaMP::Core::Dev
