#include "c_mafia_framework.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::framework {
        C_MafiaFramework *C_MafiaFramework::GetInstance() {
            return *reinterpret_cast<C_MafiaFramework **>(gPatterns.C_MafiaFramework__Instance);
        }
    }; // namespace mafia::framework
} // namespace SDK
