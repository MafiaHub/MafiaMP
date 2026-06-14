#pragma once

#include "c_character_state_handler.h"

#include <vector>

namespace SDK {
    namespace ue::game::physics {
        class C_Collision;
    }

    namespace ue::game::humanai {
        struct S_SlopeRayQuery;
        struct S_BlockedMoveShape;

        class C_CharacterStateHandlerBaseLocomotion : public C_CharacterStateHandler {
          public:
            bool Idle2MoveTransitionActive(ue::game::anim::S_WAnimStateHandle const *) const;
            static void AddRemoveSprintDescriptor(C_CharacterController *, bool sprinting);

          public:
            uint16_t m_unk64;                                                   // 0x40 - 0x42
            uint8_t m_unk66;                                                    // 0x42 - 0x43
            char _pad0[0x5];                                                    // 0x43 - 0x48
            bool m_bCanEnterAimZoom;                                            // 0x48 - 0x49
            char _pad1[0x7];                                                    // 0x49 - 0x50
            ue::game::physics::C_Collision *m_pSlopeRayCollision;               // 0x50 - 0x58
            void *m_pSlopeRayQueryVtable;                                       // 0x58 - 0x60
            uintptr_t m_unk96;                                                  // 0x60 - 0x68
            uint32_t m_unk104;                                                  // 0x68 - 0x6C
            char _pad2[0x4];                                                    // 0x6C - 0x70
            std::vector<S_SlopeRayQuery> m_SlopeRayShapes;                      // 0x70 - 0x88
            std::vector<ue::game::physics::C_Collision *> m_SlopeRayCollisions; // 0x88 - 0xA0
            uintptr_t m_unk160;                                                 // 0xA0 - 0xA8
            float m_fSlopeRayBaseHeight;                                        // 0xA8 - 0xAC
            char _pad3[0xC];                                                    // 0xAC - 0xB8
            bool m_bSlopeRayReady;                                              // 0xB8 - 0xB9
            bool m_bSlopeRayUnk;                                                // 0xB9 - 0xBA
            char _pad4[0x2];                                                    // 0xBA - 0xBC
            uint32_t m_unk188;                                                  // 0xBC - 0xC0
            std::vector<S_BlockedMoveShape> m_BlockedMoveShapes;                // 0xC0 - 0xD8
            std::vector<ue::game::physics::C_Collision *> m_BlockedMoveCollisions; // 0xD8 - 0xF0
            bool m_bSetupSlopeRayQueries;                                       // 0xF0 - 0xF1
            bool m_unk241;                                                      // 0xF1 - 0xF2
            char _pad5[0x2];                                                    // 0xF2 - 0xF4
            float m_fTurnSpeedMultiplier;                                       // 0xF4 - 0xF8
            float m_fStrafeDirection;                                           // 0xF8 - 0xFC
            int32_t m_nSlopeRayState;                                           // 0xFC - 0x100

          public:
            bool IsCanEnterAimZoom() const {
                return m_bCanEnterAimZoom;
            }
            void SetCanEnterAimZoom(bool value) {
                m_bCanEnterAimZoom = value;
            }
            float GetStrafeDirection() const {
                return m_fStrafeDirection;
            }
            void SetStrafeDirection(float value) {
                m_fStrafeDirection = value;
            }
            bool IsSlopeRayReady() const {
                return m_bSlopeRayReady;
            }
            bool IsSetupSlopeRayQueries() const {
                return m_bSetupSlopeRayQueries;
            }
        };
        static_assert(sizeof(C_CharacterStateHandlerBaseLocomotion) == 0x100, "Wrong size: C_CharacterStateHandlerBaseLocomotion");
        static_assert(offsetof(C_CharacterStateHandlerBaseLocomotion, m_bCanEnterAimZoom) == 0x48);
        static_assert(offsetof(C_CharacterStateHandlerBaseLocomotion, m_pSlopeRayCollision) == 0x50);
        static_assert(offsetof(C_CharacterStateHandlerBaseLocomotion, m_SlopeRayShapes) == 0x70);
        static_assert(offsetof(C_CharacterStateHandlerBaseLocomotion, m_bSlopeRayReady) == 0xB8);
        static_assert(offsetof(C_CharacterStateHandlerBaseLocomotion, m_BlockedMoveShapes) == 0xC0);
        static_assert(offsetof(C_CharacterStateHandlerBaseLocomotion, m_fStrafeDirection) == 0xF8);
    } // namespace ue::game::humanai
} // namespace SDK
