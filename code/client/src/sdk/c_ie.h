#pragma once

namespace SDK {
    class C_IE {
      public:
        static void *Alloc(size_t);
        static void Free(void *);
    };
} // namespace SDK