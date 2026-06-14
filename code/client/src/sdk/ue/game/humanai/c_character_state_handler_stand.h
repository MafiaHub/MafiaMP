#pragma once

#include "c_character_state_handler_base_locomotion.h"
#include "../../sys/math/c_vector.h"
#include "../../sys/utils/c_hash_name.h"

namespace SDK {
    namespace ue::game::humanai {
        // Total class size: 400 bytes (0x190)
        // Base class (C_CharacterStateHandlerBaseLocomotion): 256 bytes (0x100)
        // Stand-specific members: 144 bytes (0x90)
        class C_CharacterStateHandlerStand : public C_CharacterStateHandlerBaseLocomotion {
          public:
            enum E_StandState {
                E_SS_IDLE             = 1,
                E_SS_IDLE_TURN        = 2,
                E_SS_TURN_FROM_MOVE   = 3,
                E_SS_RESTORE          = 4,
                E_SS_WALL_BRACE       = 5,
                E_SS_SIDE_STEP        = 7
            };

            enum E_IdleMode {
                E_IM_NONE     = 0,
                E_IM_TURN     = 1,
                E_IM_SIDESTEP = 2
            };

          protected:
            ue::sys::utils::C_HashName *m_pIdleStateHash;          // 0x100 (256)
            uintptr_t m_pStoredWAnimPlaybackManager;               // 0x108 (264)
            uint64_t m_StoredGameTime;                             // 0x110 (272)
            ue::sys::math::C_Vector m_DesiredDir;                  // 0x118 (280)
            ue::sys::math::C_Vector m_DirAtStart;                  // 0x124 (292)
            float m_Speed;                                         // 0x130 (304)
            uint8_t _pad134[4];                                    // 0x134 (308)
            uint64_t _unk138;                                      // 0x138 (312)
            E_IdleMode m_IdleMode;                                 // 0x140 (320)
            uint32_t m_IdleFlags;                                  // 0x144 (324)
            ue::sys::math::C_Vector m_WallBracePos;                // 0x148 (328)
            uint8_t _pad154[4];                                    // 0x154 (340)
            ue::sys::math::C_Vector m_WallBraceNormal;             // 0x158 (344)
            uint8_t _pad164[4];                                    // 0x164 (356)
            ue::sys::math::C_Vector m_WallBraceDir;                // 0x168 (360)
            uint8_t _pad174[4];                                    // 0x174 (372)
            E_StandState m_StandState;                             // 0x178 (376)
            float m_MaxMoveThreshold;                              // 0x17C (380)
            ue::sys::math::C_Vector m_CachedDirection;             // 0x180 (384)
            uint8_t m_StateFlags;                                  // 0x18C (396)
            uint8_t _pad18D[3];                                    // 0x18D (397)

          public:
            E_StandState GetStandState() const {
                return m_StandState;
            }

            E_IdleMode GetIdleMode() const {
                return m_IdleMode;
            }

            bool HasWallBraceInfo() const {
                return (m_StateFlags & 0x02) != 0;
            }

            bool CanMoveFromForcedStop() const {
                return (m_StateFlags & 0x04) != 0;
            }

            void SetupIdleTurn(ue::sys::math::C_Vector const *dir);
            void SetupSideStep(ue::sys::math::C_Vector const *targetPos, ue::sys::math::C_Vector const *dir);
            bool CollectWallBraceInfo();
            bool CanPlayWallBraceAnim();
            void StartWallBraceAnim();
        };
    } // namespace ue::game::humanai
} // namespace SDK
