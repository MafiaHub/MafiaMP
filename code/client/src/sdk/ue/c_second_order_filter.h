#pragma once

#include <cstdint>

namespace SDK {
    namespace ue {
        // Second order dynamics filter used for smooth interpolation
        // Size: 96 bytes (0x60)
        class C_SecondOrderFilter {
          public:
            void SetSettings(float frequency, float damping, float response);

          private:
            float m_fFrequency;        // 0x00 - 0x04
            float m_fDamping;          // 0x04 - 0x08
            float m_fResponse;         // 0x08 - 0x0C
            bool m_bInitialized;       // 0x0C - 0x0D
            char _pad0[0x3];           // 0x0D - 0x10
            float m_State[4];          // 0x10 - 0x20
            float m_PrevInput[4];      // 0x20 - 0x30
            float m_Velocity[4];       // 0x30 - 0x40
            float m_Coefficients[4];   // 0x40 - 0x50
            float m_Output[4];         // 0x50 - 0x60
        };
        static_assert(sizeof(C_SecondOrderFilter) == 0x60, "Wrong size: C_SecondOrderFilter");
    } // namespace ue
} // namespace SDK
