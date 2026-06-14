#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render {
        // Description struct produced by S_GraphicsInitDesc::S_GraphicsInitDesc() (M1DE 0x143A6CF60)
        // and consumed by C_RenderCore::Init (M1DE 0x143AB76C0). Field meanings derived from
        // both the constructor defaults and the runtime values observed in the init hook log.
        //
        // The flag bits in m_nFlags are not yet fully decoded; what we know from C_RenderCore::Init:
        //   bit 0/1   -> S_RenderDeviceDesc.m_bFlagBit01
        //   bit 12    -> S_RenderDeviceDesc.m_bFlagBit12
        //   bit 14    -> referenced as "(*(_DWORD *)a2 & 0x4000) != 0" by sub_143B59F20
        //   bit 15    -> S_RenderDeviceDesc.m_bFlagBit15
        //   bit 16    -> S_RenderDeviceDesc.m_bFlagBit16
        //   bit 17    -> S_RenderDeviceDesc.m_bFlagBit17
        //   bits 4..6,3 -> "(*(_DWORD *)a2 & 0x6C) != 0" gates multi-adapter path
        //   bit 7     -> (flags & 0x80) gates an extra branch
        //   bit 11    -> (flags & 0x800) part of FullscreenWindow mask
        struct S_GraphicsInitDesc {
            uint32_t m_nFlags;                  // 0x00 - 0x04
            uint32_t m_nVideoModeIdx;           // 0x04 - 0x08  (-> S_RenderDeviceDesc.m_nVideoModeIdx)
            uint32_t m_nAdapterIdx;             // 0x08 - 0x0C  (-> S_RenderDeviceDesc.m_nAdapterIdx)
            char _pad0[0xC];                    // 0x0C - 0x18
            uint8_t m_eSwapEffect;              // 0x18 - 0x19
            char _pad1[0x17];                   // 0x19 - 0x30
            uint32_t m_nUnknown30;              // 0x30 - 0x34  (default 2)
            uint32_t m_nUnknown34;              // 0x34 - 0x38  (default 2)
            int32_t m_nUnknown38;               // 0x38 - 0x3C  (default -1)
            char _pad2[0x4];                    // 0x3C - 0x40
            char m_sShaderPath[0x20];           // 0x40 - 0x60  (std::string "/shaders")
            char m_sCachePath[0x20];            // 0x60 - 0x80  (std::string)
            char m_sExtraPath[0x20];            // 0x80 - 0xA0  (std::string)
            uint32_t m_nDefaultEnum;            // 0xA0 - 0xA4
            char _pad3[0x1C];                   // 0xA4 - 0xC0
            uint32_t m_nMinShadowRes;           // 0xC0 - 0xC4  (default 128)
            uint32_t m_nMaxShadowRes;           // 0xC4 - 0xC8  (default 128)
            uint32_t m_nBackBufferWidth;        // 0xC8 - 0xCC  (-> S_RenderDeviceDesc.m_nBackBufferWidth)
            uint32_t m_nBackBufferHeight;       // 0xCC - 0xD0  (-> S_RenderDeviceDesc.m_nBackBufferHeight)
            uint32_t m_nDisplayWidth;           // 0xD0 - 0xD4  (cached at C_RenderCore[+512])
            uint32_t m_nDisplayHeight;          // 0xD4 - 0xD8  (cached at C_RenderCore[+516])
            uint8_t m_nUnknownD8;               // 0xD8 - 0xD9  (default 0)
            uint8_t m_nUnknownD9;               // 0xD9 - 0xDA  (default 1; together with D8/DA/DB forms the dword set to 256 by the ctor)
            uint8_t m_bAsyncRenderDevice;       // 0xDA - 0xDB  (read by C_RenderCore::Init to pick async vs sync ctx)
            uint8_t m_nUnknownDB;               // 0xDB - 0xDC
            char _pad4[0x4];                    // 0xDC - 0xE0
            char _pad5[0x20];                   // 0xE0 - 0x100
        };
    } // namespace ue::sys::render
} // namespace SDK
