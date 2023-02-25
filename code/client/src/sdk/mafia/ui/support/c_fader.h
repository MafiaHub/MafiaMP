#pragma once

#include "../../../ue/c_cnt_ptr.h"

namespace SDK {
    namespace mafia::ui::support {
        class C_Fader {
          public:
            void FadeIn(ue::C_CntPtr<uintptr_t> &, float, char const *, bool);
            void FadeOut(ue::C_CntPtr<uintptr_t> &, float, char const *, bool);
            void Reset();
        };
    }; // namespace mafia::ui::support
};    // namespace SDK
