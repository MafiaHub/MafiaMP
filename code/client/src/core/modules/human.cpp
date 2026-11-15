#include <utils/safe_win32.h>

#include "human.h"

#include "vehicle.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui.h>

#include <external/imgui/widgets/nametag.h>

#include "core/application.h"

#include "game/helpers/controls.h"
#include "game/helpers/human.h"
#include "game/overrides/character_controller.h"
#include "sdk/c_game.h"
#include "sdk/c_player_teleport_module.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/game/ai/s_human_ai_request_play_anim_named.h"
#include "sdk/ue/game/anim/e_wanim_behavior_var.h"
#include "sdk/ue/game/camera/c_game_camera.h"
#include "sdk/ue/game/human/c_behavior_character.h"
#include "sdk/ue/game/humanai/c_character_state_handler.h"
#include "sdk/ue/game/vehicle/c_vehicle.h"
#include "sdk/wrappers/c_human_2_car_wrapper.h"

#include "shared/entities/register_entities.h"
#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <networking/replication/entity_registry.h>
#include <networking/replication/replication_manager.h>

#include <mafianet/bit_stream.h>
#include <mafianet/string.h>

namespace MafiaMP::Core::Modules {
    namespace {
        Human *ResolveHuman(uint64_t networkId) {
            auto *world = Framework::CoreModules::GetReplication();
            return world ? dynamic_cast<Human *>(world->GetEntityByNetworkID(networkId)) : nullptr;
        }

        // Fires when the streamed game ped is ready; binds it to its ClientHuman.
        void OnHumanRequestFinish(Game::Streaming::EntityTrackingInfo *info, bool success) {
            CreateNetCharacterController = false;
            if (!success) {
                // The factory dropped the request: forget it, so it is never returned. A failed new
                // spawn profile counts as settled, so it is not requested again every frame.
                auto *self = static_cast<Human *>(info->GetNetworkEntity());
                if (self && info == self->reskinInfo) {
                    self->reskinInfo  = nullptr;
                    self->appliedSkin = self->reskinSkin;
                }
                else if (self && info == self->info) {
                    self->info = nullptr;
                }
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

            // The ped of a new spawn profile is ready: it takes the place of the current one. The old
            // ped belongs to another spawner than the one being updated, so returning it here is safe.
            // The new ped starts on foot: one whose player sits in a car is warped into the seat, as
            // for a human streamed in already seated.
            if (info == self->reskinInfo) {
                self->ReleaseAnimation();
                Core::gApplication->GetEntityFactory()->ReturnEntity(self->info);
                const bool seated = self->data.carPassenger.carId != 0;
                self->info        = info;
                self->appliedSkin = self->reskinSkin;
                self->reskinInfo  = nullptr;
                self->enterState  = seated ? STATE_ENTERING : STATE_OUTSIDE;
                self->enterForced = seated;
                self->leaveForced = false;
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

        // Wire: <human NetworkID><animation name><loop>
        void OnHumanPlayAnimation(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint64_t id = 0;
            MafiaNet::RakString name;
            bool loop = false;
            if (!bs->Read(id) || !bs->Read(name) || !bs->Read(loop) || name.IsEmpty()) {
                return;
            }

            // Animations are transient: a human that is not spawned here simply misses it.
            auto *self = ResolveHuman(id);
            if (!self || !self->human) {
                return;
            }
            self->PlayAnimation(name.C_String(), loop);
        }

        // Wire: <human NetworkID>
        void OnHumanStopAnimations(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint64_t id = 0;
            if (!bs->Read(id)) {
                return;
            }
            auto *self = ResolveHuman(id);
            if (!self || !self->human) {
                return;
            }
            self->StopAnimation();
        }

    } // namespace

    void Human::OnConstructed() {
        if (IsOwner()) {
            BindLocalPlayer();
        }
        else {
            // A human streamed in while already seated has no enter to watch: warp it into its
            // seat as soon as its ped exists.
            if (data.carPassenger.carId != 0) {
                enterState  = STATE_ENTERING;
                enterForced = true;
            }
            info        = RequestPed();
            appliedSkin = modelHash;
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
        // Wear the spawn profile the server assigned, if it differs from the current model.
        ApplyLocalSkin();
    }

    void Human::ApplyLocalSkin() {
        // The game's player spawner takes the requested profile as soon as the switch starts, so it is
        // what to compare against, whoever switched it last.
        if (modelHash == 0 || modelHash == Game::Helpers::Controls::GetLocalPlayerSpawnProfile()) {
            return;
        }
        Game::Helpers::Controls::PlayerChangeSpawnProfile(modelHash);
    }

    void Human::OnStateForced() {
        // ReadForcedState applied the server's authoritative position/rotation; move the game ped to
        // match. We then resume streaming the ped's transform from the corrected state.
        if (isLocalPlayer) {
            // A push made for another field echoes our own pose back; only warp when the server
            // actually moved us.
            if (WasPoseForced()) {
                TeleportLocalToReplicated();
            }
            ApplyLocalSkin();
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

    Game::Streaming::EntityTrackingInfo *Human::RequestPed() {
        auto *request = Core::gApplication->GetEntityFactory()->RequestHuman(modelHash);
        interpolator.GetPosition()->SetCompensationFactor(1.5f);
        // Snapshots arrive every tick: let each one correct the whole error before the next. The
        // default range (100-400 ms) applies a quarter of it at our rate and the ped trails behind.
        // Every SetTargetValue passes the tick interval, the top of this range, so the contribution
        // is always the whole error.
        interpolator.GetPosition()->SetErrorContributionDelayRange(0.0f, Core::gApplication->GetTickIntervalMs());
        interpolator.GetRotation()->SetErrorContributionDelayRange(0.0f, Core::gApplication->GetTickIntervalMs());

        request->SetBeforeSpawnCallback([](Game::Streaming::EntityTrackingInfo *) {
            CreateNetCharacterController = true;
        });
        request->SetRequestFinishCallback(&OnHumanRequestFinish);
        request->SetReturnCallback(&OnHumanReturned);
        request->SetNetworkEntity(this);
        return request;
    }

    void Human::UpdateRemoteSkin() {
        if (isLocalPlayer || !info || modelHash == 0 || modelHash == (reskinInfo ? reskinSkin : appliedSkin)) {
            return;
        }
        const auto factory = Core::gApplication->GetEntityFactory();
        if (reskinInfo) {
            factory->ReturnEntity(reskinInfo);
            reskinInfo = nullptr;
        }
        if (modelHash == appliedSkin) {
            return;
        }
        // No ped in the world yet: nothing to keep, so the request itself is replaced.
        if (!human) {
            factory->ReturnEntity(info);
            info        = RequestPed();
            appliedSkin = modelHash;
            return;
        }
        // Keep the current ped until the new one has spawned (OnHumanRequestFinish swaps them): a
        // profile that takes long to load, or never does, leaves the player as it was, not invisible.
        reskinInfo = RequestPed();
        reskinSkin = modelHash;
    }

    void Human::DeallocReplica(MafiaNet::Connection_RM3 *) {
        ReleaseAnimation();
        if (isLocalPlayer) {
            if (Core::gApplication->GetLocalPlayer() == this) {
                Core::gApplication->SetLocalPlayer(nullptr);
            }
        }
        else {
            if (reskinInfo) {
                Core::gApplication->GetEntityFactory()->ReturnEntity(reskinInfo);
                reskinInfo = nullptr;
            }
            if (info) {
                Core::gApplication->GetEntityFactory()->ReturnEntity(info);
                info = nullptr;
            }
        }
        delete this;
    }

    void Human::SerializeFields(Framework::Networking::Replication::FieldSerializer &fields) {
        HumanEntity::SerializeFields(fields);
        if (!fields.Writing() && !isLocalPlayer) {
            ApplyRemote();
        }
    }

    void Human::PlayAnimation(char const *animationName, bool loop) {
        // A seated ped plays nothing: its car handler would be cancelled under it.
        if (!human || !charController || SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(charController->GetCurrentStateHandlerType())) {
            return;
        }
        ReleaseAnimation();

        // Build the request the play-animation AI action would and hand it to the controller
        // directly, for the local player and remote peds alike: a remote ped's AI never runs that
        // action, and one path for both keeps their animations in step (the AI restarts a looped
        // animation itself, the request loops it in the handler).
        animationRequest = SDK::game::ai::S_HumanAIRequestPlayAnimNamed::Create(animationName, loop);
        if (!animationRequest) {
            return;
        }
        SDK::ue::C_WeakPtr<SDK::game::ai::S_HumanAIRequest, SDK::ue::S_SingleThreadedPolicy, 0> request = animationRequest->GetWeakPtr();
        if (!request.Get()) {
            ReleaseAnimation();
            return;
        }
        animationRequestedAt = std::chrono::steady_clock::now();
        animationStarted     = false;

        // A remote ped's controller switches to the desired handler on every frame and drops AI
        // requests; let the game drive it while the animation plays.
        if (!isLocalPlayer) {
            charController->SetDesiredHandlerType(SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE);
            charController->SetGameDriven(true);
        }
        static_cast<SDK::ue::game::humanai::C_CharacterController *>(charController)->ProcessAILocomotionRequest(request);
    }

    void Human::StopAnimation() {
        if (!animationRequest) {
            return;
        }
        // Without its request the play-animation handler ends. A remote ped then goes back to its
        // replicated state at once rather than on the next update.
        ReleaseAnimation();
        if (!isLocalPlayer) {
            ApplyRemote();
        }
    }

    void Human::ReleaseAnimation() {
        if (!animationRequest) {
            return;
        }
        if (charController && !isLocalPlayer) {
            charController->SetGameDriven(false);
        }
        animationRequest->Release();
        animationRequest = nullptr;
    }

    bool Human::FollowAnimation() {
        if (!animationRequest) {
            return false;
        }
        if (!charController) {
            ReleaseAnimation();
            return false;
        }

        // The local ped plays it until the game ends it (finished, or interrupted by moving). A remote
        // ped follows its owner: it plays while the owner's replicated handler is the play-animation
        // one, and stops as soon as the owner heads for a car.
        bool playing = false;
        if (isLocalPlayer) {
            playing = charController->GetCurrentStateHandlerType() == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_PLAYANIM;
        }
        else {
            const auto ownerHandlerType = static_cast<SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type>(data._charStateHandlerType);
            if (data.carPassenger.carId != 0 || enterState != STATE_OUTSIDE || SDK::ue::game::humanai::C_CharacterStateHandler::IsVehicleStateHandlerType(ownerHandlerType)) {
                ReleaseAnimation();
                return false;
            }
            playing = ownerHandlerType == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_PLAYANIM;
        }

        animationStarted |= playing;
        const auto startTimeout = isLocalPlayer ? kLocalAnimationStartTimeout : kRemoteAnimationStartTimeout;
        if (playing || (!animationStarted && std::chrono::steady_clock::now() - animationRequestedAt <= startTimeout)) {
            return true;
        }
        ReleaseAnimation();
        return false;
    }

    void Human::Frame() {
        if (isLocalPlayer) {
            FollowAnimation();
            ReadLocal();
        }
        else if (human) {
            // An animation that ended between two updates hands the ped back to its replicated state.
            if (animationRequest && !FollowAnimation()) {
                ApplyRemote();
            }

            // Drive the remote ped's vehicle entry, then its interpolated transform, then UI.
            if (charController && charController->GetCurrentStateHandlerType() != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
                if (enterState == STATE_ENTERING) {
                    auto *car = Vehicle::GetByNetworkId(data.carPassenger.carId);
                    if (car && car->car) {
                        if (Game::Helpers::Human::PutIntoCar(charController, car->car, data.carPassenger.seatId, enterForced)) {
                            enterState        = STATE_INSIDE;
                            enterForced       = false;
                            carActionIssuedAt = std::chrono::steady_clock::now();
                        }
                    }
                }
                // The enter action can be accepted and still never seat the ped, for instance when
                // the car is already moving. Instead of leaving it to float beside the car, retry
                // once the action had time to finish and warp it into the seat.
                else if (enterState == STATE_INSIDE && data.carPassenger.carId != 0 && std::chrono::steady_clock::now() - carActionIssuedAt > kCarActionTimeout) {
                    enterState  = STATE_ENTERING;
                    enterForced = true;
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
            else if (charController) {
                ApplyRemoteSeatSwitch();
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

        // A seated ped stops updating its own transform, so GetPos() above is frozen at the point we
        // entered the car. This avatar is the connection's streaming viewer, so follow the car instead
        // — otherwise interest stays anchored at the entry point and the vehicle (and the player) get
        // streamed out after driving one stream radius away. The car is the one the car handler
        // drives, not the ped's owner, and the ped follows it only once it sits.
        if (data._charStateHandlerType == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
            auto *carWrapper = charController->GetCarHandler()->GetHuman2CarWrapper();
            if (carWrapper && carWrapper->GetActor() && carWrapper->GetSeatID(human) != SDK::C_Human2CarWrapper::E_SEAT_NONE) {
                const SDK::ue::sys::math::C_Vector carPos = carWrapper->GetActor()->GetPos();
                position                                  = {carPos.x, carPos.y, carPos.z};
            }
        }
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
            ReadLocalCarSeat();
        } break;
        }

        if (data._charStateHandlerType != SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR) {
            data.carPassenger = {};
        }
    }

    void Human::ReadLocalCarSeat() {
        // Report the car and seat on every frame of the car handler, not only while entering: that
        // single sample missed fast entries and never saw a seat switch. The seat is the handler's,
        // known from the start of the entry and already the driver seat for a player who slides
        // over from the passenger door; GetSeatID answers only once the ped sits.
        auto *carHandler   = charController->GetCarHandler();
        auto *carWrapper   = carHandler->GetHuman2CarWrapper();
        SDK::C_Car *car    = carWrapper ? carWrapper->GetActor() : nullptr;
        const int32_t seat = carHandler->GetCurrentSeatIndex();
        if (!car || seat == SDK::ue::game::humanai::C_CharacterStateHandlerCar::E_SEAT_NONE || carHandler->IsLeavingCar()) {
            data.carPassenger = {};
            return;
        }

        // The car reported last frame is the usual answer; only a new one needs the walk over every
        // entity GetByCar does.
        auto *carEnt = Vehicle::GetByNetworkId(data.carPassenger.carId);
        if (!carEnt || carEnt->car != car) {
            carEnt = Vehicle::GetByCar(car);
        }
        if (carEnt) {
            data.carPassenger = {carEnt->GetNetworkID(), seat};
        }
        else {
            data.carPassenger = {};
        }
    }

    bool Human::ApplyRemoteCarExit() {
        const bool inCarHandler = charController->GetCurrentStateHandlerType() == SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_CAR;
        const auto now          = std::chrono::steady_clock::now();

        // A leave ends once the ped is out of the car handler, whatever the replicated seat says by
        // then: a player who gets back in before the exit animation ends must find the ped outside,
        // not leaving for good. A leave that has not got the ped out in time is retried, forced.
        if (enterState == STATE_LEAVING) {
            if (!inCarHandler) {
                enterState  = STATE_OUTSIDE;
                leaveForced = false;
            }
            else if (now - carActionIssuedAt > kCarActionTimeout) {
                enterState  = STATE_INSIDE;
                leaveForced = true;
            }
        }

        // Leave the car once we are no longer a passenger. A ped that never got seated is outside
        // already. The car is the one the game seated the ped in: the ped's owner is not that car
        // for a ped put in through an enter action, and the leave action failed every time.
        if (data.carPassenger.carId == 0 && enterState == STATE_INSIDE) {
            if (!inCarHandler) {
                enterState  = STATE_OUTSIDE;
                leaveForced = false;
            }
            else {
                auto *carWrapper = charController->GetCarHandler()->GetHuman2CarWrapper();
                if (carWrapper && Game::Helpers::Human::RemoveFromCar(charController, carWrapper->GetActor(), leaveForced)) {
                    enterState        = STATE_LEAVING;
                    carActionIssuedAt = now;
                }
            }
        }

        return enterState == STATE_LEAVING;
    }

    void Human::ApplyRemoteSeatSwitch() {
        // A seated ped follows its player to the paired seat the way the game switches its own peds.
        // Any other seat change goes through a leave and an enter, which the replicated seat clears
        // and sets in between.
        if (enterState != STATE_INSIDE || data.carPassenger.carId == 0) {
            return;
        }
        auto *carHandler = charController->GetCarHandler();
        auto *carWrapper = carHandler->GetHuman2CarWrapper();
        auto *car        = Vehicle::GetByNetworkId(data.carPassenger.carId);
        if (!carWrapper || !car || carWrapper->GetActor() != car->car) {
            return;
        }
        const int32_t seat = data.carPassenger.seatId;
        if (carHandler->IsSeated() && carHandler->GetCurrentSeatIndex() != seat && carHandler->CanSwitchToSeat(seat)) {
            carHandler->SwitchSeat(seat);
        }
    }

    void Human::ApplyRemoteTransform() {
        // Set the interpolation target toward the replicated transform.
        interpolator.GetPosition()->SetTargetValue({human->GetPos().x, human->GetPos().y, human->GetPos().z}, position, MafiaMP::Core::gApplication->GetTickIntervalMs());
        interpolator.GetRotation()->SetTargetValue({human->GetRot().w, human->GetRot().x, human->GetRot().y, human->GetRot().z}, rotation, MafiaMP::Core::gApplication->GetTickIntervalMs());
    }

    void Human::ApplyRemote() {
        if (!human || !charController) {
            return;
        }

        // Keep remote peds from dying due to client-only factors; health comes from the owner.
        human->GetHumanScript()->SetDemigod(true);
        human->GetHumanScript()->SetInvulnerabilityByScript(true);
        MafiaMP::Game::Helpers::Human::SetHealthPercent(human, data._healthPercent);

        const auto desiredStateHandlerType = static_cast<SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type>(data._charStateHandlerType);

        // Forcing the replicated handler or move state would cancel a scripted animation: while it
        // plays, follow only the owner's transform. It ends first when the owner heads for a car.
        if (FollowAnimation()) {
            ApplyRemoteTransform();
            return;
        }

        // While the ped gets out no handler is desired: the controller switches to the desired
        // handler every frame, which would pull the ped out of its exit animation.
        if (ApplyRemoteCarExit()) {
            charController->SetDesiredHandlerType(SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE);
            return;
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

        ApplyRemoteTransform();

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
        // Capture only the stable network id, not the ped pointer: the widget runs deferred (next
        // ImGUI flush) and the human can stream out and free its ped in between, leaving a captured
        // raw pointer dangling. Re-resolve the live entity here and bail if it is gone.
        const auto networkId = GetNetworkID();
        gApplication->GetImGUI()->PushWidget([networkId]() {
            auto *self = ResolveHuman(networkId);
            if (!self || !self->human) {
                return;
            }

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
            SDK::ue::sys::math::C_Vector headPos = self->human->GetBoneWorldPos(headBoneHash);
            float distFromCam                    = headPos.dist(camPos);
            if (distFromCam <= 250.0f) {
                headPos.z += 0.335f + (distFromCam * 0.03f);

                SDK::ue::sys::math::C_Vector2 screenPos;
                bool onScreen = false;
                float unkFloat1, unkFloat2;
                camera->GetScreenPos(screenPos, headPos, onScreen, &unkFloat1, &unkFloat2, true);
                if (onScreen) {
                    const auto playerName = fmt::format("{} ({})", self->nickname.empty() ? "Player" : self->nickname, self->playerIndex);
                    Framework::External::ImGUI::Widgets::DrawNameTag(ImGui::GetBackgroundDrawList(), {screenPos.x * displaySize.x, screenPos.y * displaySize.y}, playerName.c_str(), {}, 1.0f, self->data._healthPercent);
                }
            }
        });
    }

    void Human::Install() {
        // Register the client human type for this id (overrides the server-side plain HumanEntity).
        Framework::Networking::Replication::EntityRegistry::Get().Register(Shared::Entities::HumanEntity::kTypeName, [] {
            return new Human();
        });

        auto *net = Framework::CoreModules::GetNetworkPeer();
        auto *rpc = net->GetRPC();
        // Human action RPCs received from peers.
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kHumanShoot), &OnHumanShoot, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kHumanReload), &OnHumanReload, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kHumanAddWeapon), &OnHumanAddWeapon, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kHumanPlayAnimation), &OnHumanPlayAnimation, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kHumanStopAnimations), &OnHumanStopAnimations, nullptr, 0);
    }

    void Human::UpdateAll() {
        auto *world = Framework::CoreModules::GetReplication();
        auto *repl  = world;
        if (!repl) {
            return;
        }
        repl->ForEachEntity([](Framework::Networking::Replication::NetworkEntity *e) {
            if (auto *human = dynamic_cast<Human *>(e)) {
                human->UpdateRemoteSkin();
                human->Frame();
            }
        });
    }

    Human *Human::GetByPed(SDK::C_Human2 *ptr) {
        auto *world = Framework::CoreModules::GetReplication();
        auto *repl  = world;
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
