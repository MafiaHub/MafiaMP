#pragma once

#include "s_health_segment.h"
#include "i_health_bar_listener.h"

#include <cstddef>
#include <cstdint>

namespace SDK {
    namespace mafia::health_system {

        class C_HealthBar {
          public:
            // Health segments
            S_HealthSegment **m_pHealthSegments;        // 0x00 - 0x08
            S_HealthSegment **m_pHealthSegmentsEnd;     // 0x08 - 0x10
            S_HealthSegment **m_pHealthSegmentsCap;     // 0x10 - 0x18

            // Armor segments
            S_HealthSegment **m_pArmorSegments;         // 0x18 - 0x20
            S_HealthSegment **m_pArmorSegmentsEnd;      // 0x20 - 0x28
            S_HealthSegment **m_pArmorSegmentsCap;      // 0x28 - 0x30

            void *m_pConfigData;                        // 0x30 - 0x38

            float m_fCurrentArmor;                      // 0x38 - 0x3C
            float m_fMaxArmor;                          // 0x3C - 0x40

            float m_fCurrentHealth;                     // 0x40 - 0x44
            float m_fMaxHealth;                         // 0x44 - 0x48
            float m_fTotalMaxHealth;                    // 0x48 - 0x4C

            int32_t m_nActiveHealthSegmentIndex;        // 0x4C - 0x50
            int32_t m_nActiveArmorSegmentIndex;         // 0x50 - 0x54

            float m_fRegenTimer;                        // 0x54 - 0x58
            float m_fNoDamageTimer;                     // 0x58 - 0x5C
            bool m_bInNoDamageState;                    // 0x5C - 0x5D
            char _pad0[0x3];                            // 0x5D - 0x60
            float m_fHealthToRestoreTo;                 // 0x60 - 0x64

            bool m_bInDangerZone;                       // 0x64 - 0x65
            bool m_bIsRegenerating;                     // 0x65 - 0x66
            char _pad1[0x2];                            // 0x66 - 0x68

            // Listeners
            I_HealthBarListener **m_pListeners;         // 0x68 - 0x70
            I_HealthBarListener **m_pListenersEnd;      // 0x70 - 0x78
            I_HealthBarListener **m_pListenersCap;      // 0x78 - 0x80

            // Config values
            float m_fInvulnerabilityTimer;              // 0x80 - 0x84
            float m_fInvulnerabilityDuration;           // 0x84 - 0x88
            float m_fBaseRegenRate;                     // 0x88 - 0x8C
            float m_fDangerZoneRegenRate;               // 0x8C - 0x90
            float m_fSegmentSize;                       // 0x90 - 0x94
            float m_fDangerThreshold;                   // 0x94 - 0x98
            float m_fRegenDelay;                        // 0x98 - 0x9C
            float m_fDangerRegenDelay;                  // 0x9C - 0xA0
            float m_fNoDmgTimeAfterSegmentDown;         // 0xA0 - 0xA4
            bool m_bConfigBool1;                        // 0xA4 - 0xA5
            char _pad2[0x3];                            // 0xA5 - 0xA8
            uint64_t m_uEffectParams;                   // 0xA8 - 0xB0
            float m_fConfigFloat1;                      // 0xB0 - 0xB4
            float m_fConfigFloat2;                      // 0xB4 - 0xB8
            float m_fNoDamageWaitTime;                  // 0xB8 - 0xBC
            char _pad3[0x4];                            // 0xBC - 0xC0
            bool m_bDamageGoesOverDefault;              // 0xC0 - 0xC1
            char _pad4[0x3];                            // 0xC1 - 0xC4
            float m_fConfigFloat3;                      // 0xC4 - 0xC8
            void *m_pDamageMultiplierConfig;            // 0xC8 - 0xD0
            float m_fConfigFloat4;                      // 0xD0 - 0xD4
            float m_fConfigFloat5;                      // 0xD4 - 0xD8
            bool m_bShouldNotifyUI;                     // 0xD8 - 0xD9

          public:
            float GetCurrentHealth() const {
                return m_fCurrentHealth;
            }

            float GetMaxHealth() const {
                return m_fMaxHealth;
            }

            float GetTotalMaxHealth() const {
                return m_fTotalMaxHealth;
            }

            float GetCurrentArmor() const {
                return m_fCurrentArmor;
            }

            float GetMaxArmor() const {
                return m_fMaxArmor;
            }

            bool IsInDangerZone() const {
                return m_bInDangerZone;
            }

            bool IsRegenerating() const {
                return m_bIsRegenerating;
            }

            int32_t GetActiveHealthSegmentCount() const {
                return m_nActiveHealthSegmentIndex + 1;
            }

            int32_t GetActiveArmorSegmentCount() const {
                return m_nActiveArmorSegmentIndex + 1;
            }

            size_t GetHealthSegmentCount() const {
                return (reinterpret_cast<uintptr_t>(m_pHealthSegmentsEnd) -
                        reinterpret_cast<uintptr_t>(m_pHealthSegments)) / sizeof(S_HealthSegment *);
            }

            size_t GetArmorSegmentCount() const {
                return (reinterpret_cast<uintptr_t>(m_pArmorSegmentsEnd) -
                        reinterpret_cast<uintptr_t>(m_pArmorSegments)) / sizeof(S_HealthSegment *);
            }

            S_HealthSegment *GetHealthSegment(size_t index) const {
                if (index < GetHealthSegmentCount()) {
                    return m_pHealthSegments[index];
                }
                return nullptr;
            }

            S_HealthSegment *GetArmorSegment(size_t index) const {
                if (index < GetArmorSegmentCount()) {
                    return m_pArmorSegments[index];
                }
                return nullptr;
            }

            float GetHealthPercent() const {
                if (m_fTotalMaxHealth > 0.0f) {
                    return m_fCurrentHealth / m_fTotalMaxHealth;
                }
                return 0.0f;
            }

            float GetArmorPercent() const {
                if (m_fMaxArmor > 0.0f) {
                    return m_fCurrentArmor / m_fMaxArmor;
                }
                return 0.0f;
            }

            void Update(float deltaTime, bool updateAudio);
            void FullyHealSegments(int segmentCount);
            void SetFullSegmentsCnt(uint32_t count);
            void SetMedkidInUse();
            void OnDeath();
        };

        static_assert(offsetof(C_HealthBar, m_pHealthSegments) == 0x00, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_pArmorSegments) == 0x18, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fCurrentArmor) == 0x38, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fMaxArmor) == 0x3C, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fCurrentHealth) == 0x40, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fMaxHealth) == 0x44, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fTotalMaxHealth) == 0x48, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_nActiveHealthSegmentIndex) == 0x4C, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_nActiveArmorSegmentIndex) == 0x50, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fRegenTimer) == 0x54, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fNoDamageTimer) == 0x58, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_bInNoDamageState) == 0x5C, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fHealthToRestoreTo) == 0x60, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_bInDangerZone) == 0x64, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_bIsRegenerating) == 0x65, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_pListeners) == 0x68, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fInvulnerabilityTimer) == 0x80, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fBaseRegenRate) == 0x88, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fDangerZoneRegenRate) == 0x8C, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fSegmentSize) == 0x90, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fDangerThreshold) == 0x94, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_fNoDamageWaitTime) == 0xB8, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_bDamageGoesOverDefault) == 0xC0, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_pDamageMultiplierConfig) == 0xC8, "offset mismatch");
        static_assert(offsetof(C_HealthBar, m_bShouldNotifyUI) == 0xD8, "offset mismatch");

    } // namespace mafia::health_system
} // namespace SDK
