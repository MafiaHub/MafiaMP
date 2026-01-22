#pragma once

#include "c_actor.h"

#include <cstdint>

namespace SDK {
    /**
     * C_Tree - Tree entity class
     *
     * Size: 0x128 (296 bytes)
     */
    class C_Tree: public C_Actor {
      protected:
        void *_unkF8;           // 0xF8 - 0x100
        uint32_t _unk100;       // 0x100 - 0x104
        uint8_t _unk104;        // 0x104 - 0x105
        uint8_t _pad105[3];     // 0x105 - 0x108 (padding)
        void *_unk108;          // 0x108 - 0x110
        void *_unk110;          // 0x110 - 0x118
        uint16_t _unk118;       // 0x118 - 0x11A
        uint8_t _pad11A[6];     // 0x11A - 0x120 (padding)
        void *_unk120;          // 0x120 - 0x128
    };

    static_assert(sizeof(C_Tree) == 0x128, "C_Tree size mismatch");
} // namespace SDK
