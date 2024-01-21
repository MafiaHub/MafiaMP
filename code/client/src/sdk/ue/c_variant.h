#pragma once

#include "c_string.h"

namespace SDK {
    namespace ue {
        namespace sys::core {
            enum E_ColorSetType {};
        };

        class C_Variant {
          public:
            C_Variant(C_String const &) = delete;
            ~C_Variant()                      = delete;

            C_Variant *swap(C_Variant &other) = delete;

            C_Variant *operator=(C_Variant const &other) = delete;
            C_Variant *operator=(C_Variant const &&other) = delete;
        };
    }
}
