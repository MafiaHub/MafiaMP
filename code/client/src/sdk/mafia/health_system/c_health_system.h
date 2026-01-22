#pragma once

#include "c_health_bar.h"
#include "../../ue/c_string.h"

#include <cstddef>
#include <cstdint>

namespace SDK {
    namespace mafia::health_system {

        class C_HealthBar;

        struct S_DangerZone {
            float m_fMaxDangerLevel;        // 0x00 - 0x04
            char _pad0[0x28];               // 0x04 - 0x2C
            float m_fCurrentDangerLevel;    // 0x2C - 0x30
            char _pad1[0x10];               // 0x30 - 0x40
        };

        class C_HealthSystem {
          public:
            void *m_pVtable;                    // 0x00 - 0x08
            char _pad0[0x10];                   // 0x08 - 0x18
            C_HealthBar *m_pHealthBar;          // 0x18 - 0x20
            S_DangerZone *m_pDangerZone;        // 0x20 - 0x28
            ue::C_String *m_pConfigName;        // 0x28 - 0x30
            bool m_bArmorUIEnabled;             // 0x30 - 0x31
            bool m_bHasArmor;                   // 0x31 - 0x32
            char _pad1[0x6];                    // 0x32 - 0x38

          public:
            C_HealthBar *GetHealthBar() const {
                return m_pHealthBar;
            }

            float GetArmor() const {
                if (m_pHealthBar) {
                    return m_pHealthBar->GetCurrentArmor();
                }
                return 0.0f;
            }

            float GetMaxArmor() const {
                if (m_pHealthBar) {
                    return m_pHealthBar->GetMaxArmor();
                }
                return 0.0f;
            }

            float GetCurrentHealth() const {
                if (m_pHealthBar) {
                    return m_pHealthBar->GetCurrentHealth();
                }
                return 0.0f;
            }

            float GetMaxHealth() const {
                if (m_pHealthBar) {
                    return m_pHealthBar->GetMaxHealth();
                }
                return 0.0f;
            }

            float GetDangerLevel() const {
                if (m_pDangerZone && m_pDangerZone->m_fMaxDangerLevel > 0.0f) {
                    return 1.0f - (m_pDangerZone->m_fCurrentDangerLevel / m_pDangerZone->m_fMaxDangerLevel);
                }
                return 0.5f;
            }

            bool IsInDangerZone() const {
                if (m_pHealthBar) {
                    return m_pHealthBar->IsInDangerZone();
                }
                return false;
            }

            bool HasArmor() const {
                return m_bHasArmor && GetArmor() > 0.0f;
            }

            bool IsArmorUIEnabled() const {
                return m_bArmorUIEnabled;
            }

            void CreateArmorUI();
            void UpdateArmorUI();
            void GiveArmor(float amount, bool playSound);
            void GiveFullArmor();
            void RemoveAllArmor();
            void LoadConfig();
            void ResetToMaxHealth();
            void SetHealthToSegmentCnt(uint32_t count);
            void SwitchHealthModeDbg(int mode);
            bool ValidateBulletHit(float a2, float a3);
            float GetCurrentShotDamageMultiplier() const;
        };

        static_assert(offsetof(C_HealthSystem, m_pHealthBar) == 0x18, "C_HealthSystem::m_pHealthBar offset mismatch");
        static_assert(offsetof(C_HealthSystem, m_pDangerZone) == 0x20, "C_HealthSystem::m_pDangerZone offset mismatch");
        static_assert(offsetof(C_HealthSystem, m_bArmorUIEnabled) == 0x30, "C_HealthSystem::m_bArmorUIEnabled offset mismatch");
        static_assert(offsetof(C_HealthSystem, m_bHasArmor) == 0x31, "C_HealthSystem::m_bHasArmor offset mismatch");

    } // namespace mafia::health_system
} // namespace SDK
