#pragma once

#include "e_back_buffer_format.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        struct S_VideoModeDesc {
            uint32_t m_nWidth;                  // 0x00 - 0x04
            uint32_t m_nHeight;                 // 0x04 - 0x08
            uint32_t m_nFrequency;              // 0x08 - 0x0C  (refresh rate Hz)
            E_BackBufferFormat m_eFormat;       // 0x0C - 0x10
            float m_fAspectRatio;               // 0x10 - 0x14  (width/height as float)
            char _pad0[0x20];                   // 0x14 - 0x34
        };
        static_assert(sizeof(S_VideoModeDesc) == 0x34, "S_VideoModeDesc size is invalid");
    } // namespace ue::sys::render::device
} // namespace SDK
