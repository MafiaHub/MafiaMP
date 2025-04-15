#pragma once

#include "c_keyboard.h"

namespace SDK {
    namespace ue::sys::input {
        class C_SysInput {
          public:
            C_Keyboard *m_pKeyboard;                    // 0008 - 0010

          public:
            virtual ~C_SysInput() = default;
        };
    }
}
