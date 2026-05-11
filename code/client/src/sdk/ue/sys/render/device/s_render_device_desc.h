#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        // Passed to I_RenderDevice::Init(). Built by C_RenderCore::Init from S_GraphicsInitDesc:
        //   m_nBackBufferWidth/Height  <- S_GraphicsInitDesc[+200/+204]
        //   m_nAdapterIdx              <- S_GraphicsInitDesc[+8]
        //   m_nVideoModeIdx            <- S_GraphicsInitDesc[+4]
        //   m_eSwapEffect              <- S_GraphicsInitDesc[+24]
        //   m_bFlagBit01..m_bFlagBit16 <- individual bits of S_GraphicsInitDesc.m_nFlags
        //
        // The flag bits at +17..+21 are derived from the gfx-flags bitfield. Their concrete
        // meaning is not yet confirmed from the binary, so they are exposed as named-by-bit
        // fields rather than guesses like "windowed"/"stereo" (which were wrong on first run).
        // The whole 24-byte struct is copied verbatim into C_Direct3D11RenderDevice[+8..+32]
        // by Init, so the device-side accessors (GetBackBufferWidth, GetFrequency) read
        // straight from these offsets.
        struct S_RenderDeviceDesc {
            uint32_t m_nBackBufferWidth;        // 0x00 - 0x04
            uint32_t m_nBackBufferHeight;       // 0x04 - 0x08
            uint32_t m_nAdapterIdx;             // 0x08 - 0x0C  (-> device[+16], used by GetFrequency)
            uint32_t m_nVideoModeIdx;           // 0x0C - 0x10  (-> device[+20], used by GetFrequency)
            uint8_t m_eSwapEffect;              // 0x10 - 0x11
            bool m_bFlagBit01;                  // 0x11 - 0x12  (gfx_flags & 0x3) != 0
            bool m_bFlagBit12;                  // 0x12 - 0x13  (gfx_flags & 0x1000) != 0
            bool m_bFlagBit15;                  // 0x13 - 0x14  (gfx_flags & 0x8000) != 0
            bool m_bFullscreen;                 // 0x14 - 0x15  (gfx_flags & 0x20000) != 0  - tracks S_WindowsStartSetup.m_nDisplayMode != 0
            bool m_bFlagBit16;                  // 0x15 - 0x16  (gfx_flags >> 16) & 1
            char _pad0[0x2];                    // 0x16 - 0x18
        };
        static_assert(sizeof(S_RenderDeviceDesc) == 0x18, "S_RenderDeviceDesc size is invalid");
    } // namespace ue::sys::render::device
} // namespace SDK
