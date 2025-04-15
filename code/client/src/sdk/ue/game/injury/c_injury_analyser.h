#pragma once

#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    class I_Human2;
    struct E_HitZone {};
    namespace ue::game::injury {
        class C_InjuryAnalyser {
          public:
            I_Human2 *m_pHuman;                 // 0000 - 0008
            int m_aInjuryTrackers[8];           // 0008 - 0028
            void *m_pDamageHistory;             // 0028 - 0030
            int m_iAccumulatedInjury;           // 0030 - 0034
            int m_iLastInjuryTime;              // 0034 - 0038
            float m_fInjuryMultiplier;          // 0038 - 0040
            E_HitZone m_eLastHitZone;           // 0040 - 0044
            int m_iInjuryFlag;                  // 0044 - 0048
            float m_fStunThreshold;             // 0048 - 0050
            int m_aInjuryHistory[8];            // 0050 - 0070
            void *injuryProcessingQueue[8];     // 0070 - 00B0

          public:
            void AddInjury(float, E_HitZone, sys::math::C_Vector const &);
        };
    } // namespace ue::game::human
} // namespace SDK
