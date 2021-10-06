#include "c_ie.h"
#include <utils/hooking/hooking.h>
#include "patterns.h"

namespace SDK {
    void *C_IE::Alloc(size_t size) {
        return hook::this_call<void *>(gPatterns.C_IE__AllocAddr, size);
    }

    void C_IE::Free(void *ptr) {
        hook::this_call<void>(gPatterns.C_IE__FreeAddr, ptr);
    }
} // namespace SDK
