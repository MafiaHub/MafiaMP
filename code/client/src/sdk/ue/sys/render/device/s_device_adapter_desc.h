#pragma once

#include "s_video_mode_desc.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::render::device {
        // Per-adapter descriptor stored in C_Direct3D11RenderDevice::m_Adapters.
        // Constructor zeros offsets 0x00..0x20, then fills:
        //   +0x18  VendorId   (DXGI_ADAPTER_DESC.VendorId)
        //   +0x1C  DeviceId   (DXGI_ADAPTER_DESC.DeviceId)
        //   +0x20  SubSysId   (DXGI_ADAPTER_DESC.SubSysId)
        //   +0x24  Revision   (DXGI_ADAPTER_DESC.Revision)
        //   +0x28  vendor-class enum: 0=NVIDIA (0x10DE), 1=AMD (0x1002), 2=other
        //
        // The fake software-fallback adapter (vendor 0x1414 / "Fake SW adapter") is the
        // only producer that populates the m_pName / m_pDescription pointers at +0x08 /
        // +0x10. For real DXGI adapters those two qwords stay zero; the engine resolves
        // a display name from VendorId/DeviceId elsewhere.
        struct S_DeviceAdapterDesc {
            char _pad0[0x8];                    // 0x00 - 0x08
            const char *m_pName;                // 0x08 - 0x10  (only set for fake SW adapter)
            const char *m_pDescription;         // 0x10 - 0x18  (only set for fake SW adapter)
            uint32_t m_nVendorId;               // 0x18 - 0x1C
            uint32_t m_nDeviceId;               // 0x1C - 0x20
            uint32_t m_nSubSysId;               // 0x20 - 0x24
            uint32_t m_nRevision;               // 0x24 - 0x28
            uint32_t m_eVendorClass;            // 0x28 - 0x2C  (0=NVIDIA, 1=AMD, 2=other)
            uint32_t m_nUnknown2C;              // 0x2C - 0x30
            S_VideoModeDesc *m_pVideoModesBegin;    // 0x30 - 0x38  (std::vector<S_VideoModeDesc>)
            S_VideoModeDesc *m_pVideoModesEnd;      // 0x38 - 0x40
            S_VideoModeDesc *m_pVideoModesCapacity; // 0x40 - 0x48

            uint32_t GetNumModes() const {
                return static_cast<uint32_t>(m_pVideoModesEnd - m_pVideoModesBegin);
            }

            const S_VideoModeDesc *GetMode(uint32_t i) const {
                return m_pVideoModesBegin + i;
            }
        };
        static_assert(sizeof(S_DeviceAdapterDesc) == 0x48, "S_DeviceAdapterDesc size is invalid");
    } // namespace ue::sys::render::device
} // namespace SDK
