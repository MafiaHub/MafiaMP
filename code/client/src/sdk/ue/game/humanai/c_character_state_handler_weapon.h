#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    namespace ue::game::humanai {
        // Size: ~216 bytes (0xD8)
        class C_CharacterStateHandlerWeapon : public C_CharacterStateHandler {
          protected:
            uint16_t m_unk64;                 // 0x40 - 0x42
            uint8_t m_unk66;                  // 0x42 - 0x43
            char _pad0[0x5];                  // 0x43 - 0x48
            uintptr_t m_unk72;                // 0x48 - 0x50
            uintptr_t m_unk80;                // 0x50 - 0x58
            uintptr_t m_unk88;                // 0x58 - 0x60
            uintptr_t m_unk96;                // 0x60 - 0x68
            int64_t m_nWeaponStateHash1;      // 0x68 - 0x70 (default -1)
            int64_t m_nWeaponStateHash2;      // 0x70 - 0x78 (default -1)
            int32_t m_nWeaponSlot;            // 0x78 - 0x7C (default -1)
            uint32_t m_unk124;                // 0x7C - 0x80
            uint32_t m_unk128;                // 0x80 - 0x84
            uint16_t m_unk132;                // 0x84 - 0x86
            char _pad1[0x2];                  // 0x86 - 0x88
            uintptr_t m_unk136;               // 0x88 - 0x90
            uintptr_t m_unk144;               // 0x90 - 0x98
            uint32_t m_nReloadState;          // 0x98 - 0x9C
            float m_fAimShakeReduce;          // 0x9C - 0xA0 (default 1.0f)
            uint32_t m_unk160;                // 0xA0 - 0xA4
            uint32_t m_nAimShakeState;        // 0xA4 - 0xA8
            uintptr_t m_unk168;               // 0xA8 - 0xB0
            uintptr_t m_unk176;               // 0xB0 - 0xB8
            uintptr_t m_unk184;               // 0xB8 - 0xC0
            uintptr_t m_unk192;               // 0xC0 - 0xC8
            uintptr_t m_unk200;               // 0xC8 - 0xD0
            uintptr_t m_unk208;               // 0xD0 - 0xD8

          public:
            bool CanChangeWeapon(bool) const;
            bool CanReload() const;
            bool ChamberingWeapon() const;
            uint32_t GetNextWeaponID(uint32_t, bool, bool);
            void SelectWeapon(bool);
            void SetupReload(bool);
            void SetupWeaponChange(uint32_t, bool, bool);
            void SetupFire();
            void SetupForcedWeaponDrop();
            void StopReload();
            void SwapWeaponHands();
            void UpdateAimShakeReduce();
            void UpdateBlindFire();
            void UpdateReload(float);
            void UpdateWeaponDescriptors();
            void PickupItem();
            void PlaySyncroAnimOnWeapon();
            void HandleAimShakeReduceStateMaxReduction();
            void HandleAimShakeReduceStateReducing();
        };
    } // namespace ue::game::humanai
} // namespace SDK
