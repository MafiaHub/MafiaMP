#pragma once

#include "c_character_state_handler.h"
#include "../../sys/math/c_vector.h"

namespace SDK {
    namespace ue {
        namespace game {
            namespace physics {
                class C_Collision;
            }
        }
    }

    namespace ue::game::humanai {
        /**
         * @brief Character state handler for aiming mechanics
         * 
         * Manages all aiming behavior including:
         * - Camera and entity-based aiming
         * - Aim direction calculation and smoothing
         * - Blind fire states
         * - Weapon swap detection
         * - Collision-based aim blocking
         * - Animation state management
         * 
         * @note This handler is active whenever the character is in aim mode
         * @note Supports both player-controlled and AI-controlled aiming
         */
        class C_CharacterStateHandlerAim : public C_CharacterStateHandler {
          public:
            /**
             * @brief Aim targeting mode
             */
            enum E_AimMode {
                E_AIM_CAMERA = 0,    ///< Aim where camera is looking
                E_AIM_ENTITY = 1,    ///< Track specific entity
                E_AIM_POSITION = 2   ///< Aim at fixed world position
            };

            /**
             * @brief Aim blocked state
             */
            enum E_AimBlockedState {
                E_AIM_NOT_BLOCKED = 0,     ///< Aim is not blocked
                E_AIM_BLOCKED = 1,         ///< Aim is blocked by collision
                E_AIM_FORCE_BLOCKED = 2    ///< Aim is force blocked
            };

          protected:
            // Member variables (offsets from base class end at 0x40/64)
            uint16_t m_Flags;                              // +0x40 (64)
            uint8_t  m_Flag;                               // +0x42 (66)
            uint8_t  pad_67;                               // +0x43 (67)
            uint32_t pad_68;                               // +0x44 (68)
            E_AimMode m_AimMode;                           // +0x48 (72) - 0=camera, 1=entity, 2=position
            uint32_t pad_76;                               // +0x4C (76)
            void    *m_pTargetEntityWeakPtr;               // +0x50 (80) - C_WeakPtr to target entity
            void    *m_pSecondaryTargetWeakPtr;            // +0x58 (88) - C_WeakPtr to secondary target
            ue::sys::math::C_Vector m_AimDirection;        // +0x60 (96) - Aim direction vector
            float    m_AimYaw;                             // +0x6C (108) - Current aim yaw angle
            float    m_AimPitch;                           // +0x70 (112) - Current aim pitch angle
            float    m_CurrentRotation;                    // +0x74 (116) - Current rotation angle
            bool     m_bInCover;                           // +0x78 (120) - Is character in cover
            bool     m_bBlindFire;                         // +0x79 (121) - Is in blind fire mode
            bool     m_bUnknownState;                      // +0x7A (122) - Unknown state flag
            uint8_t  pad_123;                              // +0x7B (123)
            float    m_UnkFloat124;                        // +0x7C (124)
            float    m_UnkFloat128;                        // +0x80 (128)
            uint32_t pad_132;                              // +0x84 (132)
            void    *m_pWeaponInventory;                   // +0x88 (136) - Weapon inventory pointer
            float    m_YawSmoothingBuffer[10];             // +0x90 (144) - Smoothing buffer for yaw
            float    m_PitchSmoothingBuffer[10];           // +0xB8 (184) - Smoothing buffer for pitch
            int32_t  m_SmoothingFrameCounter;              // +0xE0 (224) - Frame counter for smoothing
            E_AimBlockedState m_AimBlockedState;           // +0xE4 (228) - Aim blocked state
            void    *m_pAnimationState;                    // +0xE8 (232) - Animation state handle
            void    *m_pBlindFireAnimState;                // +0xF0 (240) - Blind fire animation state
            void    *m_pWeaponSwapAnimState;               // +0xF8 (248) - Weapon swap animation state
            void    *m_pAnimState256;                      // +0x100 (256)
            void    *m_pAnimState264;                      // +0x108 (264)
            void    *m_pAnimState272;                      // +0x110 (272)
            ue::sys::math::C_Vector m_TargetPosition;      // +0x118 (280) - Target position vector
            ue::sys::math::C_Vector m_SecondaryPosition;   // +0x124 (292) - Secondary position vector
            uint32_t pad_304;                              // +0x130 (304)
            uint32_t pad_308;                              // +0x134 (308)
            uint16_t m_CollisionID;                        // +0x138 (312) - Unique collision ID
            uint16_t pad_314;                              // +0x13A (314)
            uint32_t pad_316;                              // +0x13C (316)
            ue::game::physics::C_Collision *m_pCollision;  // +0x140 (320) - Collision object for aim ray
            void    *m_pPhysicsSubsystemVtable;            // +0x148 (328) - Physics subsystem vtable
            void    *m_pUnk336;                            // +0x150 (336)
            uint32_t m_Unk344;                             // +0x158 (344)
            uint32_t pad_348;                              // +0x15C (348)
            uint8_t  m_CollisionFlags;                     // +0x160 (352)
            uint8_t  m_UnkFlag353;                         // +0x161 (353)

          public:
            // Virtual method overrides
            virtual ~C_CharacterStateHandlerAim() override = default;
            virtual bool Activate(void *aiRequest) override;
            virtual void Deactivate() override;
            virtual void UpdateRequest(void *aiRequest) override;
            virtual void UpdateAIFreq(void *aiRequest) override;
            virtual bool UpdateHumanFreq(float deltaTime) override;
            virtual void UpdateHumanFreqPostPhysics(float deltaTime) override;
            virtual void Start() override;
            virtual void Finish(C_CharacterStateHandler *nextHandler) override;
            virtual void AcceptMessage(void *message) override;
            virtual void UpdatePlayerInput(S_PlayerControllerInput *input, void *playerState) override;
            virtual void OnCharacterControllerActivated() override;
            virtual void OnCharacterControllerDeactivated() override;
            virtual void OnStartBlindFire() override;

            // Public methods
            /**
             * @brief Check if character is currently swapping weapons
             * @return True if weapon swap animation is active
             */
            bool SwappingWeapon();

            /**
             * @brief Update aim animation state
             * @return True if animation was updated successfully
             */
            bool UpdateAimAnimation();

            /**
             * @brief Check if aiming is currently allowed
             * @return True if character can aim under current conditions
             * @note Checks for blocking conditions like being in vehicle, climbing, etc.
             */
            bool IsAimAllowed() const;

            /**
             * @brief Check if aim is blocked by environment
             * @return True if aim is blocked by collision
             * @note Only relevant for player characters (type 9)
             */
            bool IsAimBlocked() const;

            /**
             * @brief Check if weapon reload is currently blocked
             * @return True if reload is blocked
             */
            bool IsReloadBlocked() const;

            /**
             * @brief Update aim direction with smoothing
             * @param deltaTime Time delta for interpolation
             * @return True if aim direction was updated
             * @note Handles angle wrapping and interpolation
             */
            bool UpdateAimDirection(float deltaTime);

            /**
             * @brief Calculate player aim yaw and pitch from camera
             * @param outYaw Pointer to output yaw angle
             * @param outPitch Pointer to output pitch angle
             * @param outMatrix Reference to output transformation matrix
             * @param cameraMatrix Camera transformation matrix
             * @return Status code
             * @note Uses smoothing buffer for stable aiming
             */
            bool CalculatePlayerAimYawPitch(float *outYaw, float *outPitch, 
                                           ue::sys::math::C_Matrix &outMatrix,
                                           ue::sys::math::C_Matrix &cameraMatrix);

            /**
             * @brief Calculate aim angles for free aim and blind fire
             * @param outYaw Pointer to output yaw angle
             * @param outPitch Pointer to output pitch angle
             * @param isBlindFire True if calculating for blind fire mode
             * @return Aim state code
             */
            int CalculatePlayerAimYawPitchFreeAndBlindFire(float *outYaw, float *outPitch, bool isBlindFire) const;

            /**
             * @brief Update player aim blocked state based on collision
             * @param cameraMatrix Camera transformation matrix
             * @param characterMatrix Character transformation matrix
             * @note Performs raycast to detect obstructions
             */
            void UpdatePlayerAimBlocked(const ue::sys::math::C_Matrix &cameraMatrix,
                                       const ue::sys::math::C_Matrix &characterMatrix);

            /**
             * @brief Update AI-controlled character aiming
             * @param deltaTime Time delta
             * @return True if updated successfully
             */
            bool UpdateHumanFreqAI(float deltaTime);
        };
    } // namespace ue::game::humanai
} // namespace SDK
