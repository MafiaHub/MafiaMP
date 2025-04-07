#pragma once

#include <cstdint>

#include "c_input_device_specialization.h"

namespace SDK {
    namespace ue::sys::input {
        class C_SysInput;

        class C_Keyboard {
          public:
            uint32_t m_iFlags;                              // 0008 - 000C
            char pad0[4];                                   // 000C - 0010
            C_SysInput *m_pSysInput;                        // 0010 - 0018
            C_InputDeviceSpecialization *m_pSpecialization; // 0018 - 0020
            char pad1[120];                                 // 0020 - 0098
            void *m_pKeyMap;                                // 0098 - 00A0
            char pad2[2408];                                // 00A0 - 0A48
            void *m_pSomeStruct;                            // 0A48 - 0A50
            char pad3[1120];                                // 0A50 - 1350
            uint64_t m_iSomeState1;                         // 1350 - 1358
            uint64_t m_iSomeState2;                         // 1358 - 1360
            float m_fScalar1;                               // 1360 - 1364
            float m_fScalar2;                               // 1364 - 1368
            bool m_bShiftPressed;                           // 1368 - 1369
            bool m_bCtrlPressed;                            // 1369 - 136A
            bool m_bAltPressed;                             // 136A - 136B
            char pad4[1];                                   // 136B - 136C
            uint16_t m_iUnk1;                               // 136C - 136E
            char pad5[2];                                   // 136E - 1370

          public:
            virtual ~C_Keyboard() = default;
            uint32_t GetActiveModifiers() const;
            int64_t ToString(char *);
        };
    }
}
