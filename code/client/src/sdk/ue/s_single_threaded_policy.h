#pragma once

namespace SDK {
    namespace ue {
        // Threading policy tag of the engine's smart pointers (ue::C_WeakPtr<T, S_SingleThreadedPolicy, N>).
        struct S_SingleThreadedPolicy {};
    } // namespace ue
} // namespace SDK
