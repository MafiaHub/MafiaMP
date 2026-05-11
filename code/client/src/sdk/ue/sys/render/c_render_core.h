#pragma once

#include "device/i_render_device.h"
#include "s_graphics_init_desc.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render {
        namespace device {
            class C_DynamicVIBufferPool;
            class I_RenderDeviceCtx;
        }
        class C_RenderContext;

        // Top-level rendering façade owned by C_Core (m_pRenderCore). Sets up the
        // render device, creates the default render destination, and stores cached
        // copies of the display configuration. Field offsets reverse-engineered from
        // C_RenderCore::Init (M1DE 0x143AB76C0) and CreateDefaultRenderDestination
        // (M1DE 0x143A913A0).
        class C_RenderCore {
          public:
            char _pad0[0x8];                                    // 0x000 - 0x008  (vtable)
            void *m_pRenderBackend;                             // 0x008 - 0x010
            void *m_pUnknown10;                                 // 0x010 - 0x018
            void *m_pSettingsListener;                          // 0x018 - 0x020
            char _pad1[0x8];                                    // 0x020 - 0x028
            void *m_pPipeline;                                  // 0x028 - 0x030
            void *m_pUnknown30;                                 // 0x030 - 0x038
            device::I_RenderDevice *m_pRenderDevice;            // 0x038 - 0x040
            device::I_RenderDeviceCtx *m_pPrimaryCtx;           // 0x040 - 0x048
            device::I_RenderDeviceCtx *m_pAsyncCtx;             // 0x048 - 0x050
            device::C_DynamicVIBufferPool *m_pDynamicVIBufferPool;   // 0x050 - 0x058
            void *m_pFrameSink;                                 // 0x058 - 0x060
            C_RenderContext *m_pDefaultRenderContext;           // 0x060 - 0x068  (CreateDefaultRenderDestination writes here)
            char _pad2[0x38];                                   // 0x068 - 0x0A0
            char _pad3[0x60];                                   // 0x0A0 - 0x100
            char _pad4[0x10];                                   // 0x100 - 0x110
            char _pad5[0xF0];                                   // 0x110 - 0x200
            uint32_t m_nDisplayWidth;                           // 0x200 - 0x204  (= S_GraphicsInitDesc.m_nDisplayWidth)
            uint32_t m_nDisplayHeight;                          // 0x204 - 0x208  (= S_GraphicsInitDesc.m_nDisplayHeight)
            uint32_t m_nBackBufferHeight;                       // 0x208 - 0x20C  (cached I_RenderDevice::GetBackBufferHeight)
            uint32_t m_nBackBufferWidth;                        // 0x20C - 0x210  (cached I_RenderDevice::GetBackBufferWidth)
            float m_fDisplayAspect;                             // 0x210 - 0x214  (cached I_RenderDevice::GetDisplayAspect)
            uint8_t m_MonitorDesc[0x14];                        // 0x214 - 0x228  (cached MainMonitorDesc payload)
            uint32_t m_nNumAdapters;                            // 0x228 - 0x22C
            uint32_t m_nNumAdapterModes;                        // 0x22C - 0x230
            bool m_bHasMainMonitor;                             // 0x230 - 0x231
            char _pad6[0x1F];                                   // 0x231 - 0x250
            uint8_t m_bUseAsyncDevice;                          // 0x250 - 0x251  (mirrors S_GraphicsInitDesc.m_bAsyncRenderDevice)
            char _pad7[0xF];                                    // 0x251 - 0x260
            void *m_pDeviceCallbacks;                           // 0x260 - 0x268

            inline device::I_RenderDevice *GetRenderDevice() const { return m_pRenderDevice; }
            inline uint32_t GetDisplayWidth() const { return m_nDisplayWidth; }
            inline uint32_t GetDisplayHeight() const { return m_nDisplayHeight; }
            inline uint32_t GetBackBufferWidth() const { return m_nBackBufferWidth; }
            inline uint32_t GetBackBufferHeight() const { return m_nBackBufferHeight; }
            inline float GetDisplayAspect() const { return m_fDisplayAspect; }
            inline bool HasMainMonitor() const { return m_bHasMainMonitor; }
        };
    } // namespace ue::sys::render
} // namespace SDK
