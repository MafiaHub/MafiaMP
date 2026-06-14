#pragma once

#include <sdk/e_damage_type.h>
#include <sdk/ue/sys/math/c_vector.h>

#include "c_entity_message.h"

namespace SDK {
    class C_EntityMessageDamage : public C_EntityMessage {
      public:
        ue::sys::math::C_Vector m_vSourcePosition;      // 0020 - 0038
        ue::sys::math::C_Vector m_vHitDirection;        // 0x38 - 0x50: Direction vector
        void *m_pUnk;                                   // 0x38 - 0x40: Unknown pointer (set to 0)
        float m_fDamageAmount;                          // 0x40 - 0x44: Base damage amount (initialized to 1.0f)
        float m_fDamageAmount1;                         // 0x44 - 0x48: Damage amount 1
        float m_fDamageAmount2;                         // 0x48 - 0x4C: Damage amount 2
        E_DamageType m_iDamageType;                     // 0x4C - 0x50: Type of damage
        unsigned int m_iSourceEntityID;                 // 0x50 - 0x54: Source entity ID
        int m_iUnk;                                     // 0x54 - 0x58: Unknown integer (set to 0)
        void *m_pData;                                  // 0x58 - 0x60: Additional data pointer
        int m_iUnk2;                                    // 0x60 - 0x64: Unknown integer
        short m_sUnk3;                                  // 0x64 - 0x66: Unknown short
        int m_iUnk4;                                    // 0x68 - 0x6C: Unknown integer (set to -1)
        bool m_bIsCritical;                             // 0x6C - 0x6D: Critical hit flag
        short m_sUnk5;                                  // 0x6D - 0x6F: Unknown short (set to 257)
        int m_iUnk5;                                    // 0x70 - 0x74: Unknown integer (set to 0)
        ue::sys::math::C_Vector m_vImpactPosition;      // 0x74 - 0x8C: Impact position
        bool m_bIsFatal;                                // 0x80 - 0x81: Fatal damage flag
        bool m_bUnk;                                    // 0x81 - 0x82: Unknown flag
    };
}
