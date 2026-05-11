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
        class C_RenderDeviceCtxAccessor;
        class I_RenderDeviceCtx;
        class I_RenderPlatformSpecificData;
        class I_DeviceTexture;

        // Base interface implemented by C_RenderDeviceNULL and C_Direct3D11RenderDevice.
        // The vtable order below is reconstructed from the C_Direct3D11RenderDevice vtable
        // at 0x144FAEFC0 (M1DE) and cross-referenced with the demangled M3 Mac symbols.
        // Only methods we use directly are kept named; the rest are placeholders to keep
        // the vtable slot indices stable when calling Init/Reset/Present through the iface.
        class I_RenderDevice {
          public:
            // --- TypeInfo / Name (slots 0..2)
            virtual void *GetClassTypeInfo()  = 0;
            virtual void *GetTypeInfo()       = 0;
            virtual const char *GetName()     = 0;

            // --- Adapter enumeration (slots 3..6)
            virtual uint32_t GetNumAdapters() const                                       = 0;
            virtual const S_DeviceAdapterDesc *GetAdapterDesc(uint32_t adapter) const     = 0;
            virtual uint32_t GetNumAdapterModes(uint32_t adapter) const                   = 0;
            virtual const S_VideoModeDesc *GetVideoModeDesc(uint32_t adapter, uint32_t mode) const = 0;

            // --- Lifecycle (slots 7..9)
            virtual void *Unknown7()          = 0;
            virtual bool Init(const S_RenderDeviceDesc &desc, C_DynamicVIBufferPool &pool, I_RenderDeviceCtx *&outCtx) = 0;
            virtual void Done()               = 0;

            // --- Internal pipeline hooks (slots 10..11)
            virtual void *Unknown10()         = 0;
            virtual void *Unknown11()         = 0;

            // --- Back buffer / display state (slots 12..18)
            virtual I_DeviceTexture *GetBackBufferTexture(const I_RenderPlatformSpecificData *platformData) = 0;
            virtual uint32_t GetBackBufferWidth() const  = 0;
            virtual uint32_t GetBackBufferHeight() const = 0;
            virtual E_BackBufferFormat GetBackBufferFormat() const = 0;
            virtual uint32_t GetFrequency() const = 0;
            virtual float GetDisplayAspect() const = 0;
            virtual void OnRenderDestinationCreated(const I_RenderPlatformSpecificData *platformData, uint32_t width, uint32_t height) = 0;

            // --- Monitor info (slot 19)
            virtual void *GetMainMonitorDesc() const = 0;

            // --- Destination teardown / present / reset (slots 20+)
            virtual void OnRenderDestinationDestroyed(const I_RenderPlatformSpecificData *platformData) = 0;
            virtual void Reset(const S_RenderDeviceDesc &desc) = 0;
            virtual void Present(C_RenderDeviceCtxAccessor &accessor, const void *clipRect, const I_RenderPlatformSpecificData *platformData, E_VSyncMode vsync, bool blocking) = 0;

            // Non-virtual helper baked into I_RenderDevice (M1DE 0x143B10BE0).
            // Computed live from GetBackBufferWidth() / GetBackBufferHeight().
            float IsHalHighResMode() const {
                return static_cast<float>(GetBackBufferWidth()) /
                       static_cast<float>(GetBackBufferHeight());
            }
        };

    } // namespace ue::sys::render::device
} // namespace SDK
