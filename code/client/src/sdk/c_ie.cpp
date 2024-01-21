#include "c_ie.h"
#include "patterns.h"

namespace SDK {
    void *C_IE::Alloc(size_t size) {
        return hook::this_call<void *>(gPatterns.C_IE__Alloc, size);
    }

    void C_IE::Free(void *ptr) {
        hook::this_call<void>(gPatterns.C_IE__Free, ptr);
    }
} // namespace SDK
