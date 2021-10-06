#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::anim {
        struct S_WAnimStateHandle {
            uintptr_t unk0;

            S_WAnimStateHandle(uintptr_t __unk0 = 0): unk0(__unk0) {}
        };
    } // namespace ue::game::anim
} // namespace SDK
