#pragma once

#include "c_mafia_framework_interfaces.h"

namespace SDK {
    namespace mafia::framework {
        class C_MafiaFramework {
          public:
            C_MafiaFrameworkInterfaces *GetInterfaces() {
                return &m_FrameworkInterfaces;
            }
            static C_MafiaFramework *GetInstance();

          private:
            float m_fUnk1;
            float m_fUnk2;
            C_MafiaFrameworkInterfaces m_FrameworkInterfaces;
        };
    }; // namespace mafia::framework
    static_assert(sizeof(mafia::framework::C_MafiaFramework) == 96, "Wrong size: mafia::framework::C_MafiaFramework");
} // namespace SDK
