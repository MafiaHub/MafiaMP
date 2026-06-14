#pragma once

#include <cstddef>
#include <cstdint>

namespace SDK {
    namespace mafia::health_system {

        struct S_HealthSegment {
            bool m_bActive;             // 0x00 - 0x01
            char _pad0[0x3];            // 0x01 - 0x04
            float m_fSize;              // 0x04 - 0x08
            float m_fRegenRate;         // 0x08 - 0x0C
            float m_fCurrentHealth;     // 0x0C - 0x10
            float m_fNoDmgTime;         // 0x10 - 0x14
            char _pad1[0x4];            // 0x14 - 0x18
            float m_fEffectParam1;      // 0x18 - 0x1C
            float m_fEffectParam2;      // 0x1C - 0x20
            float m_fNotifyThreshold;   // 0x20 - 0x24
            bool m_bDamageGoesOver;     // 0x24 - 0x25
            bool m_bIsArmorSegment;     // 0x25 - 0x26
            char _pad2[0x2];            // 0x26 - 0x28

            bool IsActive() const {
                return m_bActive;
            }

            float GetSize() const {
                return m_fSize;
            }

            float GetCurrentHealth() const {
                return m_fCurrentHealth;
            }

            float GetRegenRate() const {
                return m_fRegenRate;
            }

            bool IsFullyHealed() const {
                return m_fCurrentHealth >= m_fSize;
            }
        };

        static_assert(sizeof(S_HealthSegment) == 0x28, "S_HealthSegment size mismatch");
        static_assert(offsetof(S_HealthSegment, m_bActive) == 0x00, "S_HealthSegment::m_bActive offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_fSize) == 0x04, "S_HealthSegment::m_fSize offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_fRegenRate) == 0x08, "S_HealthSegment::m_fRegenRate offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_fCurrentHealth) == 0x0C, "S_HealthSegment::m_fCurrentHealth offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_fNoDmgTime) == 0x10, "S_HealthSegment::m_fNoDmgTime offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_fNotifyThreshold) == 0x20, "S_HealthSegment::m_fNotifyThreshold offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_bDamageGoesOver) == 0x24, "S_HealthSegment::m_bDamageGoesOver offset mismatch");
        static_assert(offsetof(S_HealthSegment, m_bIsArmorSegment) == 0x25, "S_HealthSegment::m_bIsArmorSegment offset mismatch");

    } // namespace mafia::health_system
} // namespace SDK
