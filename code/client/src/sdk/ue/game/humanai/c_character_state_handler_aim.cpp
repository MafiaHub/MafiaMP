#include "c_character_state_handler_aim.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::humanai {
        // Virtual method overrides

        bool C_CharacterStateHandlerAim::Activate(void *aiRequest) {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__Activate, this, aiRequest);
        }

        void C_CharacterStateHandlerAim::Deactivate() {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__Deactivate, this);
        }

        void C_CharacterStateHandlerAim::UpdateRequest(void *aiRequest) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__UpdateRequest, this, aiRequest);
        }

        void C_CharacterStateHandlerAim::UpdateAIFreq(void *aiRequest) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__UpdateAIFreq, this, aiRequest);
        }

        bool C_CharacterStateHandlerAim::UpdateHumanFreq(float deltaTime) {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__UpdateHumanFreq, this, deltaTime);
        }

        void C_CharacterStateHandlerAim::UpdateHumanFreqPostPhysics(float deltaTime) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__UpdateHumanFreqPostPhysics, this, deltaTime);
        }

        void C_CharacterStateHandlerAim::Start() {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__Start, this);
        }

        void C_CharacterStateHandlerAim::Finish(C_CharacterStateHandler *nextHandler) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__Finish, this, nextHandler);
        }

        void C_CharacterStateHandlerAim::AcceptMessage(void *message) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__AcceptMessage, this, message);
        }

        void C_CharacterStateHandlerAim::UpdatePlayerInput(S_PlayerControllerInput *input, void *playerState) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__UpdatePlayerInput, this, input, playerState);
        }

        void C_CharacterStateHandlerAim::OnCharacterControllerActivated() {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__OnCharacterControllerActivated, this);
        }

        void C_CharacterStateHandlerAim::OnCharacterControllerDeactivated() {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__OnCharacterControllerDeactivated, this);
        }

        void C_CharacterStateHandlerAim::OnStartBlindFire() {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__OnStartBlindFire, this);
        }

        // Public methods

        bool C_CharacterStateHandlerAim::SwappingWeapon() {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__SwappingWeapon, this);
        }

        bool C_CharacterStateHandlerAim::UpdateAimAnimation() {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__UpdateAimAnimation, this);
        }

        bool C_CharacterStateHandlerAim::IsAimAllowed() const {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__IsAimAllowed, this);
        }

        bool C_CharacterStateHandlerAim::IsAimBlocked() const {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__IsAimBlocked, this);
        }

        bool C_CharacterStateHandlerAim::IsReloadBlocked() const {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__IsReloadBlocked, this);
        }

        bool C_CharacterStateHandlerAim::UpdateAimDirection(float deltaTime) {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__UpdateAimDirection, this, deltaTime);
        }

        bool C_CharacterStateHandlerAim::CalculatePlayerAimYawPitch(float *outYaw, float *outPitch,
                                                                    ue::sys::math::C_Matrix &outMatrix,
                                                                    ue::sys::math::C_Matrix &cameraMatrix) {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__CalculatePlayerAimYawPitch,
                                        this, outYaw, outPitch, &outMatrix, &cameraMatrix);
        }

        int C_CharacterStateHandlerAim::CalculatePlayerAimYawPitchFreeAndBlindFire(float *outYaw, float *outPitch,
                                                                                   bool isBlindFire) const {
            return hook::this_call<int>(gPatterns.C_CharacterStateHandlerAim__CalculatePlayerAimYawPitchFreeAndBlindFire,
                                       this, outYaw, outPitch, isBlindFire);
        }

        void C_CharacterStateHandlerAim::UpdatePlayerAimBlocked(const ue::sys::math::C_Matrix &cameraMatrix,
                                                                const ue::sys::math::C_Matrix &characterMatrix) {
            hook::this_call<void>(gPatterns.C_CharacterStateHandlerAim__UpdatePlayerAimBlocked,
                                 this, &cameraMatrix, &characterMatrix);
        }

        bool C_CharacterStateHandlerAim::UpdateHumanFreqAI(float deltaTime) {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerAim__UpdateHumanFreqAI, this, deltaTime);
        }

    } // namespace ue::game::humanai
} // namespace SDK
