#include <utils/safe_win32.h>

#include "human.h"

#include "vehicle.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui/imgui.h>

#include <external/imgui/widgets/nametag.h>

#include "core/application.h"

#include "game/helpers/controls.h"
#include "game/helpers/human.h"
#include "game/overrides/character_controller.h"
#include "sdk/c_game.h"
#include "sdk/c_player_teleport_module.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/ue/game/anim/e_wanim_behavior_var.h"
#include "sdk/ue/game/human/c_behavior_character.h"
#include "sdk/ue/game/humanai/c_character_state_handler.h"
#include "sdk/ue/game/vehicle/c_vehicle.h"
#include "sdk/ue/game/camera/c_game_camera.h"
#include "sdk/wrappers/c_human_2_car_wrapper.h"

#include "shared/entities/register_entities.h"
#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <networking/replication/entity_factory.h>
#include <networking/replication/replication_manager.h>
#include <world/engine.h>

#include <mafianet/BitStream.h>

namespace MafiaMP::Core::Modules {
    namespace {
        Human *ResolveHuman(uint64_t networkId) {
            auto *world = Framework::CoreModules::GetWorldEngine();
            return world ? dynamic_cast<Human *>(world->GetEntityByNetworkID(networkId)) : nullptr;
        }

        // Fires when the streamed game ped is ready; binds it to its ClientHuman.
        void OnHumanRequestFinish(Game::Streaming::EntityTrackingInfo *info, bool success) {
            CreateNetCharacterController = false;
            if (!success) {
                return;
            }
            auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
            if (!human) {
                return;
            }
            human->GameInit();
            human->Activate();

            auto *self = static_cast<Human *>(info->GetNetworkEntity());
            if (!self) {
                return;
            }

            SDK::ue::sys::math::C_Vector newPos    = {self->position.x, self->position.y, self->position.z};
            SDK::ue::sys::math::C_Quat newRot      = {self->rotation.x, self->rotation.y, self->rotation.z, self->rotation.w};
            SDK::ue::sys::math::C_Matrix transform = {};
            transform.Identity();
            transform.SetRot(newRot);
            transform.SetPos(newPos);
            human->SetTransform(transform);

            self->human          = human;
            self->charController = reinterpret_cast<MafiaMP::Game::Overrides::CharacterController *>(human->GetCharacterController());
            assert(MafiaMP::Game::Overrides::CharacterController::IsInstanceOfClass(self->charController));

            // Apply whatever state has arrived so far.
            self->Frame();
        }

        void OnHumanReturned(Game::Streaming::EntityTrackingInfo *info, bool wasCreated) {
            if (!info) {
                return;
            }
            auto human = reinterpret_cast<SDK::C_Human2 *>(info->GetEntity());
            if (wasCreated && human) {
                human->Deactivate();
                human->GameDone();
                human->Release();
            }
        }

        // --- Raw RPC4 event handlers ---

        // Wire: <human NetworkID><aimPos><aimDir><unk0><unk1>
        void OnHumanShoot(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint64_t id = 0;
            glm::vec3 aimPos {}, aimDir {};
            bool unk0 = false, unk1 = false;
            bs->Read(id);
            bs->Read(aimPos);
            bs->Read(aimDir);
            bs->Read(unk0);
            bs->Read(unk1);

            auto *self = ResolveHuman(id);
            if (!self || !self->human) {
                return;
            }
            const auto wepController = self->human->GetHumanWeaponController();
            if (!wepController) {
                return;
            }
            SDK::ue::sys::math::C_Vector pos = {aimPos.x, aimPos.y, aimPos.z};
            SDK::ue::sys::math::C_Vector dir = {aimDir.x, aimDir.y, aimDir.z};
            wepController->DoShot(nullptr, &pos, &dir, unk0, unk1);
        }

        // Wire: <human NetworkID><unk0>
        void OnHumanReload(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint64_t id = 0;
            int unk0    = 0;
            bs->Read(id);
            bs->Read(unk0);

            auto *self = ResolveHuman(id);
            if (!self || !self->human) {
                return;
            }
            const auto wepController = self->human->GetHumanWeaponController();
            if (wepController) {
                wepController->DoWeaponReloadInventory(unk0);
            }
        }

        // Wire: <human NetworkID><weaponId><ammo>
        void OnHumanAddWeapon(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint64_t id = 0;
            int weaponId = 0, ammo = 0;
            bs->Read(id);
            bs->Read(weaponId);
            bs->Read(ammo);

            auto *self = ResolveHuman(id);
            if (!self || !self->human) {
                return;
            }
            self->human->GetInventoryWrapper()->AddWeapon(weaponId, ammo);
        }

    } // namespace

    bool Human::IsOwnedByUs() const {
        const auto *manager = static_cast<const Framework::Networking::Replication::ReplicationManager *>(replicaManager);
        return manager && ownerGUID == manager->GetMyGUID();
    }

    void Human::OnConstructed() {
        if (IsOwnedByUs()) {
            BindLocalPlayer();
        }
        else {
            RequestPed();
        }
    }

    void Human::BindLocalPlayer() {
        isLocalPlayer = true;
        human         = Game::Helpers::Controls::GetLocalPlayer();
        if (human) {
            charController = reinterpret_cast<MafiaMP::Game::Overrides::CharacterController *>(human->GetCharacterController());
        }
        Core::gApplication->SetLocalPlayer(this);
        // Adopt the server-assigned spawn that arrived with construction.
        TeleportLocalToReplicated();
    }

    void Human::OnStateForced() {
        // ReadForcedState applied the server's authoritative position/rotation; move the game ped to
        // match. We then resume streaming the ped's transform from the corrected state.
        if (isLocalPlayer) {
            TeleportLocalToReplicated();
        }
    }

    void Human::TeleportLocalToReplicated() {
        if (!human) {
            return;
        }
        SDK::ue::sys::math::C_Vector newPos    = {position.x, position.y, position.z};
        SDK::ue::sys::math::C_Quat newRot      = {rotation.x, rotation.y, rotation.z, rotation.w};
        SDK::ue::sys::math::C_Matrix transform = {};
        transform.Identity();
        transform.SetRot(newRot);
        transform.SetPos(newPos);

        // TeleportPlayer preloads the world (collisions) and moves the car the player is in. It needs
        // a direction, which we derive from the rotation.
        glm::mat4 rotMatrix                 = glm::mat4_cast(rotation);
        SDK::ue::sys::math::C_Vector newDir = {-rotMatrix[1][0], rotMatrix[1][1], rotMatrix[1][2]};
        SDK::ue::C_CntPtr<uintptr_t> syncObject;
        SDK::GetPlayerTeleportModule()->TeleportPlayer(syncObject, newPos, newDir, true, true, true, false);

        human->SetTransform(transform);
    }

    void Human::RequestPed() {
        info = Core::gApplication->GetEntityFactory()->RequestHuman(modelHash);
        interpolator.GetPosition()->SetCompensationFactor(1.5f);

        info->SetBeforeSpawnCallback([](Game::Streaming::EntityTrackingInfo *) {
            CreateNetCharacterController = true;
        });
        info->SetRequestFinishCallback(&OnHumanRequestFinish);
        info->SetReturnCallback(&OnHumanReturned);
        info->SetNetworkEntity(this);
    }

    void Human::DeallocReplica(MafiaNet::Connection_RM3 *) {
        if (isLocalPlayer) {
            if (Core::gApplication->GetLocalPlayer() == this) {
                Core::gApplication->SetLocalPlayer(nullptr);
            }
        }
        else if (info) {
            Core::gApplication->GetEntityFactory()->ReturnEntity(info);
            info = nullptr;
        }
        delete this;
    }

    void Human::DeserializeFields(MafiaNet::VariableDeltaSerializer *vds, MafiaNet::VariableDeltaSerializer::DeserializationContext *ctx) {
        HumanEntity::DeserializeFields(vds, ctx);
        if (!isLocalPlayer) {
            ApplyRemote();
        }
    }

    void Human::Frame() {
        if (isLocalPlayer) {
            ReadLocal();
        }
        else if (human) {
            // Drive the remote ped's vehicle entry, then its interpolated transform, then UI.
            if (charController && charController->GetCurrentStateHandlerType() != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                if (enterState == STATE_ENTERING) {
                    auto *car = Vehicle::GetByNetworkId(data.carPassenger.carId);
                    if (car && car->car) {
                        if (Game::Helpers::Human::PutIntoCar(charController, car->car, data.carPassenger.seatId, enterForced)) {
                            enterState  = STATE_INSIDE;
                            enterForced = false;
                        }
                    }
                }
                else if (!SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(charController->GetCurrentStateHandlerType())) {
                    const auto humanPos = human->GetPos();
                    const auto humanRot = human->GetRot();
                    const auto newPos   = interpolator.GetPosition()->UpdateTargetValue({humanPos.x, humanPos.y, humanPos.z});
                    const auto newRot   = interpolator.GetRotation()->UpdateTargetValue({humanRot.w, humanRot.x, humanRot.y, humanRot.z});
                    human->SetPos({newPos.x, newPos.y, newPos.z});
                    human->SetRot({newRot.x, newRot.y, newRot.z, newRot.w});
                }
            }
            DrawNametag();
        }
    }

    void Human::ReadLocal() {
        if (!human || !charController) {
            return;
        }

        const SDK::ue::sys::math::C_Vector pedPos = human->GetPos();
        const SDK::ue::sys::math::C_Quat pedRot   = human->GetRot();
        position = {pedPos.x, pedPos.y, pedPos.z};
        rotation = {pedRot.w, pedRot.x, pedRot.y, pedRot.z};

        data._healthPercent        = MafiaMP::Game::Helpers::Human::GetHealthPercent(human);
        data._charStateHandlerType = charController->GetCurrentStateHandler()->GetStateHandlerType();
        data._isStalking           = charController->IsStalkMove();
        data._isSprinting          = charController->IsSprinting();
        data._sprintSpeed          = charController->GetSprintMoveSpeed();

        auto weaponController = human->GetHumanWeaponController();
        if (weaponController) {
            SDK::ue::sys::math::C_Vector aimDir;
            weaponController->GetAimDir(&aimDir);
            data.weaponData.aimDir          = {aimDir.x, aimDir.y, aimDir.z};
            data.weaponData.currentWeaponId = weaponController->GetRightHandWeaponID();
            data.weaponData.isAiming        = weaponController->IsAiming();
            data.weaponData.isFiring        = weaponController->m_bFirePressed;
        }

        auto currentHandler = charController->GetCurrentStateHandler();
        if (currentHandler) {
            auto behaviorChar = currentHandler->GetBehaviorCharacter();
            if (behaviorChar) {
                for (size_t i = 0; i < Shared::Modules::WANIM_VAR_SYNC_COUNT; ++i) {
                    data._animVars[i] = behaviorChar->GetWAnimVariable(static_cast<SDK::ue::game::anim::E_WAnimBehaviorVar>(i));
                }
            }
        }

        switch (data._charStateHandlerType) {
        case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_MOVE: {
            SDK::E_HumanMoveMode hmm = charController->GetHumanMoveMode();
            data._moveMode           = hmm != SDK::E_HumanMoveMode::E_HMM_NONE ? static_cast<uint8_t>(hmm) : (uint8_t)-1;
        } break;

        case SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR: {
            auto human2CarWrapper = charController->GetCarHandler()->GetHuman2CarWrapper();
            if (human2CarWrapper && charController->GetCarHandler()->GetCarState() == 2) /* entering */ {
                auto *car         = (SDK::C_Car *)human->GetOwner();
                auto *carEnt      = Vehicle::GetByCar(car);
                if (carEnt) {
                    data.carPassenger = {carEnt->GetNetworkID(), (int)human2CarWrapper->GetSeatID(human)};
                }
            }
            else if (human2CarWrapper && charController->GetCarHandler()->GetCarState() == 8) /* leaving */ {
                data.carPassenger = {};
            }
        } break;
        }

        if (data._charStateHandlerType != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
            data.carPassenger = {};
        }
    }

    void Human::ApplyRemote() {
        if (!human || !charController) {
            return;
        }

        // Apply a replicated skin change.
        if (modelHash != appliedSkin) {
            appliedSkin = modelHash;
            // TODO: re-skin a remote ped (local player handled by the game directly).
        }

        // Keep remote peds from dying due to client-only factors; health comes from the owner.
        human->GetHumanScript()->SetDemigod(true);
        human->GetHumanScript()->SetInvulnerabilityByScript(true);
        MafiaMP::Game::Helpers::Human::SetHealthPercent(human, data._healthPercent);

        const auto desiredStateHandlerType = static_cast<SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type>(data._charStateHandlerType);

        // Leave the car if we are no longer a passenger.
        if (data.carPassenger.carId == 0 && enterState == STATE_INSIDE) {
            enterState = STATE_LEAVING;
            if (Game::Helpers::Human::RemoveFromCar(charController, (SDK::C_Car *)human->GetOwner(), false)) {
                enterState = STATE_OUTSIDE;
            }
        }

        if (SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(desiredStateHandlerType)) {
            charController->SetDesiredHandlerType(SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE);
            if (desiredStateHandlerType == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                if (charController->GetCurrentStateHandlerType() != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                    if (data.carPassenger.carId > 0 && enterState == STATE_OUTSIDE) {
                        enterState = STATE_ENTERING;
                    }
                }
            }
            return;
        }

        // Set the interpolation target toward the replicated transform.
        interpolator.GetPosition()->SetTargetValue({human->GetPos().x, human->GetPos().y, human->GetPos().z}, position, MafiaMP::Core::gApplication->GetTickInterval());
        interpolator.GetRotation()->SetTargetValue({human->GetRot().w, human->GetRot().x, human->GetRot().y, human->GetRot().z}, rotation, MafiaMP::Core::gApplication->GetTickInterval());

        charController->SetDesiredHandlerType(desiredStateHandlerType);
        charController->SetStalkMoveOverride(data._isStalking);
        const auto hmm = data._moveMode != (uint8_t)-1 ? static_cast<SDK::E_HumanMoveMode>(data._moveMode) : SDK::E_HumanMoveMode::E_HMM_NONE;
        charController->SetMoveStateOverride(hmm, data._isSprinting, data._sprintSpeed);

        auto currentHandler = charController->GetCurrentStateHandler();
        if (currentHandler) {
            auto behaviorChar = currentHandler->GetBehaviorCharacter();
            if (behaviorChar) {
                for (size_t i = 0; i < Shared::Modules::WANIM_VAR_SYNC_COUNT; ++i) {
                    behaviorChar->SetWAnimVariable(static_cast<SDK::ue::game::anim::E_WAnimBehaviorVar>(i), data._animVars[i]);
                }
            }
        }

        const auto wepController = human->GetHumanWeaponController();
        if (wepController) {
            if (wepController->GetRightHandWeaponID() != data.weaponData.currentWeaponId) {
                wepController->DoWeaponSelectByItemId(data.weaponData.currentWeaponId, true);
            }
            SDK::ue::C_CntPtr<uintptr_t> syncObject2;
            human->GetHumanScript()->ScrAim(syncObject2, data.weaponData.isAiming);
            wepController->SetAiming(data.weaponData.isAiming);
            wepController->SetFirePressedFlag(data.weaponData.isFiring);
        }
    }

    void Human::DrawNametag() {
        const auto humanPtr  = human;
        const auto hp        = data._healthPercent;
        const auto nick      = nickname;
        const auto playerId  = playerIndex;
        gApplication->GetImGUI()->PushWidget([humanPtr, hp, nick, playerId]() {
            const auto displaySize = ImGui::GetIO().DisplaySize;

            auto gameCamera = SDK::ue::game::camera::C_GameCamera::GetInstanceInternal();
            if (!gameCamera) {
                return;
            }
            auto camera = gameCamera->GetCamera(SDK::ue::game::camera::E_GameCameraID::CAMERA_PLAYER_MAIN);
            if (!camera) {
                return;
            }

            auto camPos                          = camera->GetPos();
            static const auto headBoneHash       = SDK::ue::sys::utils::C_HashName::ComputeHash("Head");
            SDK::ue::sys::math::C_Vector headPos = humanPtr->GetBoneWorldPos(headBoneHash);
            float distFromCam                    = headPos.dist(camPos);
            if (distFromCam <= 250.0f) {
                headPos.z += 0.335f + (distFromCam * 0.03f);

                SDK::ue::sys::math::C_Vector2 screenPos;
                bool onScreen = false;
                float unkFloat1, unkFloat2;
                camera->GetScreenPos(screenPos, headPos, onScreen, &unkFloat1, &unkFloat2, true);
                if (onScreen) {
                    const auto playerName = fmt::format("{} ({})", nick.empty() ? "Player" : nick, playerId);
                    Framework::External::ImGUI::Widgets::DrawNameTag({screenPos.x * displaySize.x, screenPos.y * displaySize.y}, playerName.c_str(), hp);
                }
            }
        });
    }

    void Human::Install() {
        // Register the client human type for this id (overrides the server-side plain HumanEntity).
        Framework::Networking::Replication::EntityFactory::Get().Register(Shared::Entities::HumanEntity::kTypeName, [] {
            return new Human();
        });

        auto *rpc = Framework::CoreModules::GetNetworkPeer()->GetRPC();
        // Human action RPCs received from peers.
        rpc->RegisterSlot(Shared::RPC::kHumanShoot, &OnHumanShoot, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kHumanReload, &OnHumanReload, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kHumanAddWeapon, &OnHumanAddWeapon, nullptr, 0);
    }

    void Human::UpdateAll() {
        auto *world = Framework::CoreModules::GetWorldEngine();
        auto *repl  = world ? world->GetReplication() : nullptr;
        if (!repl) {
            return;
        }
        repl->ForEachEntity([](Framework::Networking::Replication::NetworkEntity *e) {
            if (auto *human = dynamic_cast<Human *>(e)) {
                human->Frame();
            }
        });
    }

    Human *Human::GetByPed(SDK::C_Human2 *ptr) {
        auto *world = Framework::CoreModules::GetWorldEngine();
        auto *repl  = world ? world->GetReplication() : nullptr;
        if (!repl || !ptr) {
            return nullptr;
        }
        Human *found = nullptr;
        repl->ForEachEntity([&](Framework::Networking::Replication::NetworkEntity *e) {
            if (found) {
                return;
            }
            if (auto *human = dynamic_cast<Human *>(e); human && human->human == ptr) {
                found = human;
            }
        });
        return found;
    }
} // namespace MafiaMP::Core::Modules
