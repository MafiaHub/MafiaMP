#pragma once

#include <vector>

namespace SDK {
    namespace ue {
        class C_FsmState;

        class C_StatesRegister {
          public:
            std::vector<C_FsmState *> _states; // 0008 - 0020
            C_FsmState *_pDefaultState;        // 0020 - 0028

          public:
            virtual ~C_StatesRegister() = default;

            void DeleteStates();
        };

        static_assert(sizeof(C_StatesRegister) == 0x28, "C_StatesRegister size mismatch");

    } // namespace ue
} // namespace SDK
