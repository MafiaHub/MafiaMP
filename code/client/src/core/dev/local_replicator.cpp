#include <utils/safe_win32.h>

#include "local_replicator.h"

#include <logging/logger.h>

#include "core/application.h"
#include "game/helpers/controls.h"
#include "game/helpers/human.h"
#include "game/streaming/entity_factory.h"
#include "sdk/c_game.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/human/c_human_script.h"
#include "sdk/ue/game/human/c_behavior_character.h"
#include "sdk/ue/game/humanai/c_character_controller.h"
#include "sdk/ue/game/humanai/c_character_state_handler.h"
#include "sdk/wrappers/c_human_2_car_wrapper.h"

extern thread_local bool CreateNetCharacterController;

namespace MafiaMP::Core::Dev {

    void LocalReplicator::SpawnAndStartReplication(uint64_t spawnProfile) {
        // Stop existing replication first
        if (IsActive()) {
            StopAndDespawn();
        }

        _trackingInfo = Core::gApplication->GetEntityFactory()->RequestHuman(spawnProfile);

        const auto OnHumanRequestFinish = [this](Game::Streaming::EntityTrackingInfo *info, bool success) {
            CreateNetCharacterController = false;
            if (success) {
                _human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
                if (!_human) {
                    return;
                }

                _human->GameInit();
                _human->Activate();

                // Get the custom character controller
                _charController = reinterpret_cast<Game::Overrides::CharacterController *>(_human->GetCharacterController());
                if (!Game::Overrides::CharacterController::IsInstanceOfClass(_charController)) {
                    Framework::Logging::GetLogger("LocalReplicator")->warn("CharacterController is not our override class!");
                    _charController = nullptr;
                }

                // Position at player with offset
                auto localPlayer = SDK::GetGame()->GetActivePlayer();
                if (localPlayer) {
                    SDK::ue::sys::math::C_Vector playerPos = localPlayer->GetPos();
                    SDK::ue::sys::math::C_Quat playerRot   = localPlayer->GetRot();

                    // Apply offset
                    playerPos.x += _positionOffset.x;
                    playerPos.y += _positionOffset.y;
                    playerPos.z += _positionOffset.z;

                    SDK::ue::sys::math::C_Matrix transform = {};
                    transform.Identity();
                    transform.SetRot(playerRot);
                    transform.SetPos(playerPos);
                    _human->SetTransform(transform);
                }

                // Set as demigod so it doesn't die from client-side factors
                _human->GetHumanScript()->SetDemigod(true);
                _human->GetHumanScript()->SetInvulnerabilityByScript(true);

                // Give the same weapons as player (for testing)
                Game::Helpers::Human::AddWeapon(_human, 85, 200);
                Game::Helpers::Human::AddWeapon(_human, 3, 200);
                Game::Helpers::Human::AddWeapon(_human, 13, 200);

                // Setup interpolator
                _interpolator.GetPosition()->SetCompensationFactor(1.5f);

                Framework::Logging::GetLogger("LocalReplicator")->info("Local replication started");
            }
        };

        const auto OnHumanReturned = [this](Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
            if (!info) {
                return;
            }
            auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
            if (wasCreated && human) {
                human->Deactivate();
                human->GameDone();
                human->Release();
            }
            _human          = nullptr;
            _charController = nullptr;
        };

        _trackingInfo->SetBeforeSpawnCallback([](Game::Streaming::EntityTrackingInfo *) {
            CreateNetCharacterController = true;
        });
        _trackingInfo->SetRequestFinishCallback(OnHumanRequestFinish);
        _trackingInfo->SetReturnCallback(OnHumanReturned);
    }

    void LocalReplicator::StopAndDespawn() {
        if (_trackingInfo) {
            Core::gApplication->GetEntityFactory()->ReturnEntity(_trackingInfo);
            _trackingInfo   = nullptr;
            _human          = nullptr;
            _charController = nullptr;
            Framework::Logging::GetLogger("LocalReplicator")->info("Local replication stopped");
        }
    }

    void LocalReplicator::Update() {
        if (!IsActive()) {
            return;
        }

        // Step 1: Collect local player state (same as UpdateLocalPlayer ECS system)
        CollectLocalPlayerState();

        // Step 2: Apply state to our replicated human (same as Human::Update)
        ApplyStateToHuman();

        // Step 3: Update interpolation (same as UpdateRemoteHuman ECS system)
        UpdateInterpolation();
    }

    void LocalReplicator::CollectLocalPlayerState() {
        auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!localPlayer) {
            return;
        }

        // Compute position and rotation from local player
        SDK::ue::sys::math::C_Vector newPos = localPlayer->GetPos();
        SDK::ue::sys::math::C_Quat newRot   = localPlayer->GetRot();

        // Apply offset to position
        _targetPos = {newPos.x + _positionOffset.x, newPos.y + _positionOffset.y, newPos.z + _positionOffset.z};
        _targetRot = {newRot.w, newRot.x, newRot.y, newRot.z};

        // Get character controller data
        auto charController            = localPlayer->GetCharacterController();
        _syncData._healthPercent       = Game::Helpers::Human::GetHealthPercent(localPlayer);
        _syncData._charStateHandlerType = charController->GetCurrentStateHandler()->GetStateHandlerType();
        _syncData._isStalking          = charController->IsStalkMove();
        _syncData._isSprinting         = charController->IsSprinting();
        _syncData._sprintSpeed         = charController->GetSprintMoveSpeed();

        // Weapon controller data
        auto weaponController = localPlayer->GetHumanWeaponController();
        if (weaponController) {
            SDK::ue::sys::math::C_Vector aimDir;
            weaponController->GetAimDir(&aimDir);
            _syncData.weaponData.aimDir          = {aimDir.x, aimDir.y, aimDir.z};
            _syncData.weaponData.currentWeaponId = weaponController->GetRightHandWeaponID();
            _syncData.weaponData.isAiming        = weaponController->IsAiming();
            _syncData.weaponData.isFiring        = weaponController->m_bFirePressed;
        }

        // Current state-specific sync data
        switch (_syncData._charStateHandlerType) {
        case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_MOVE: {
            SDK::E_HumanMoveMode hmm = charController->GetHumanMoveMode();
            uint8_t moveMode        = hmm != SDK::E_HumanMoveMode::E_HMM_NONE ? static_cast<uint8_t>(hmm) : (uint8_t)-1;
            _syncData._moveMode     = moveMode;

            // Get the animation speed (WAnimVariable 0) for walk/run blend
            auto currentHandler = charController->GetCurrentStateHandler();
            if (currentHandler && currentHandler->GetBehaviorCharacter()) {
                _syncData._animMoveSpeed = currentHandler->GetBehaviorCharacter()->GetWAnimVariable(0);
            }
        } break;

        case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR: {
            // For local replication, we don't sync vehicle state
            // as the temp human isn't in the same vehicle
        } break;
        }

        // Clear car passenger data for local replication (not applicable)
        if (_syncData._charStateHandlerType != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
            _syncData.carPassenger = {};
        }
    }

    void LocalReplicator::ApplyStateToHuman() {
        if (!_human || !_charController) {
            return;
        }

        const auto desiredStateHandlerType = static_cast<SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type>(_syncData._charStateHandlerType);

        // Skip vehicle state handlers for local replication
        if (SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(desiredStateHandlerType)) {
            _charController->SetDesiredHandlerType(SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE);
            return;
        }

        // Set target position for interpolation
        const auto humanPos = _human->GetPos();
        const auto humanRot = _human->GetRot();
        _interpolator.GetPosition()->SetTargetValue({humanPos.x, humanPos.y, humanPos.z}, _targetPos, Core::gApplication->GetTickInterval());
        _interpolator.GetRotation()->SetTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z}, _targetRot, Core::gApplication->GetTickInterval());

        // Update human based data
        _charController->SetDesiredHandlerType(desiredStateHandlerType);
        _charController->SetStalkMoveOverride(_syncData._isStalking);
        const auto hmm = _syncData._moveMode != (uint8_t)-1 ? static_cast<SDK::E_HumanMoveMode>(_syncData._moveMode) : SDK::E_HumanMoveMode::E_HMM_NONE;
        _charController->SetMoveStateOverride(hmm, _syncData._isSprinting, _syncData._sprintSpeed);
        _charController->SetAnimMoveSpeed(_syncData._animMoveSpeed);

        // Weapon inventory sync
        const auto wepController = _human->GetHumanWeaponController();
        if (wepController) {
            if (wepController->GetRightHandWeaponID() != _syncData.weaponData.currentWeaponId) {
                wepController->DoWeaponSelectByItemId(_syncData.weaponData.currentWeaponId, true);
            }

            // Aiming state sync
            SDK::ue::C_CntPtr<uintptr_t> syncObject2;
            _human->GetHumanScript()->ScrAim(syncObject2, _syncData.weaponData.isAiming);
            wepController->SetAiming(_syncData.weaponData.isAiming);

            // Shooting state sync
            wepController->SetFirePressedFlag(_syncData.weaponData.isFiring);
        }
    }

    void LocalReplicator::UpdateInterpolation() {
        if (!_human) {
            return;
        }

        const auto humanPos = _human->GetPos();
        const auto humanRot = _human->GetRot();
        const auto newPos   = _interpolator.GetPosition()->UpdateTargetValue({humanPos.x, humanPos.y, humanPos.z});
        const auto newRot   = _interpolator.GetRotation()->UpdateTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z});

        _human->SetPos({newPos.x, newPos.y, newPos.z});
        _human->SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
    }

    void LocalReplicator::ToggleAim() {
        if (!_human) {
            return;
        }

        if (auto wepCtrl = _human->GetHumanWeaponController()) {
            SDK::ue::C_CntPtr<uintptr_t> syncObject2;
            _human->GetHumanScript()->ScrAim(syncObject2, !wepCtrl->IsAiming());
            Framework::Logging::GetLogger("LocalReplicator")->debug("Aiming: {}", wepCtrl->IsAiming());
        }
    }

} // namespace MafiaMP::Core::Dev
