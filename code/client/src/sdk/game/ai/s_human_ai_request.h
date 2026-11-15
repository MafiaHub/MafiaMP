#pragma once

#include <cstddef>

#include "sdk/ue/c_base_object.h"
#include "sdk/ue/c_ref_ptr_counter.h"
#include "sdk/ue/c_weak_ptr.h"

namespace SDK {
    namespace game::ai {
        // What a human's AI asks its character controller for; the controller activates the state
        // handler that serves it and keeps only weak pointers to it. Intrusively counted.
        class S_HumanAIRequest: public ue::C_BaseObject, public ue::C_RefPtrCounter {
          public:
            ue::C_WeakPtr<S_HumanAIRequest, ue::S_SingleThreadedPolicy, 0> GetWeakPtr();

          private:
            char _pad18[0x18]; // 0x18 - 0x30

            static void AssertLayout() {
                static_assert(sizeof(S_HumanAIRequest) == 0x30, "S_HumanAIRequest size mismatch");
            }
        };
    } // namespace game::ai
} // namespace SDK
