#pragma once

#include "e_back_buffer_format.h"
#include "e_vsync_mode.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {

        struct S_RenderDeviceDesc;
        struct S_DeviceAdapterDesc;
        struct S_VideoModeDesc;
        class C_DynamicVIBufferPool;
        class I_RenderDeviceCtx;
        class I_RenderPlatformSpecificData;
        class I_DeviceTexture;

        // Base interface for the render device (C_RenderDeviceNULL / C_Direct3D11RenderDevice).
        // We do NOT declare these as C++ virtuals: the binary's MSVC vtable layout puts the
        // scalar deleting destructor at slot 0, whereas a C++ class declared with virtual
        // methods (and no virtual dtor) would put the first method at slot 0. Mixing the
        // two would dispatch e.g. GetBackBufferWidth() to the binary's destructor.
        //
        // Instead we expose a single _vftable pointer field so derived classes inherit the
        // correct 8-byte head, and document the binary's vtable layout below.
        //
        // M1DE C_Direct3D11RenderDevice vtable (vptr -> 0x144FAF0E0):
        //   slot  0: scalar deleting destructor                  sub_143AFA570
        //   slot  1: GetClassTypeInfo                            0x143B0F700
        //   slot  2: GetTypeInfo                                 0x143B16CF0
        //   slot  3: GetName                                     0x143B14C20
        //   slot  4: GetNumAdapters                              0x143B14CB0
        //   slot  5: GetAdapterDesc(adapter)                     0x143B0CD10
        //   slot  6: GetNumAdapterModes(adapter)                 0x143B14C60
        //   slot  7: GetVideoModeDesc(adapter, mode)             0x143B17190
        //   slot  8: GetGPUVendorSpecific (reads this+0x38)      0x143B10F30
        //   slot  9: Init(desc, pool, &outCtx)                   0x143B17D50
        //   slot 10: Done                                        0x143B08860
        //   slot 11: OnRenderDestinationCreated                  0x143B1C510
        //   slot 12: OnRenderDestinationDestroyed                0x143B1C550
        //   slot 13: GetBackBufferTexture                        0x143B0EFE0
        //   slot 14: IsHalHighResMode (M1DE-only, default 0)     0x143B0EFD0
        //   slot 15: GetBackBufferWidth                          0x143B0F050
        //   slot 16: GetBackBufferHeight                         0x143B0EFA0
        //   slot 17: GetBackBufferFormat (returns 21)            0x143B0EF80
        //   slot 18: GetFrequency                                0x143B10EA0
        //   slot 19: GetDisplayAspect                            0x143B10BE0
        //   slot 20: GetMainMonitorDesc (M1DE: stubbed -> 0)     0x143B19DC0
        //   slot 21: nullsub                                     0x143B22CC0
        //   slot 22: builds 20-byte monDesc {0,0,W,H,aspect}     0x143B120E0
        //   slot 23: returns 1                                   0x143B14D50
        //   slot 24: returns 1                                   0x143B14D60
        //   slot 25: returns false                               0x143B0F070
        //   ... (further slots not yet reversed)
        //
        // C_RenderCore::Init drives the device through vtable byte offsets:
        //   +72 (slot 9)  Init
        //   +136 (slot 17) GetBackBufferFormat -> cached at C_RenderCore[+524]
        //   +144 (slot 18) GetFrequency        -> cached at C_RenderCore[+520]
        //   +152 (slot 19) GetDisplayAspect    -> cached at C_RenderCore[+528]
        //   +176 (slot 22) build monDesc       -> 20 bytes copied to C_RenderCore[+532..+551]
        //   +184 (slot 23)                     -> cached at C_RenderCore[+552]
        //   +192 (slot 24)                     -> cached at C_RenderCore[+556]
        //   +200 (slot 25)                     -> cached at C_RenderCore[+560] (bool)
        class I_RenderDevice {
          public:
            void *_vftable;                     // 0x00 - 0x08

            float ComputeDisplayAspect(uint32_t backBufferWidth, uint32_t backBufferHeight) const {
                return backBufferHeight ? static_cast<float>(backBufferWidth) / static_cast<float>(backBufferHeight) : 0.0f;
            }
        };
        static_assert(sizeof(I_RenderDevice) == 0x8, "I_RenderDevice size is invalid");
    } // namespace ue::sys::render::device
} // namespace SDK
