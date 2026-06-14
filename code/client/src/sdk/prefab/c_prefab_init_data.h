#pragma once

#include "../ue/sys/utils/c_guid.h"

#include <cstdint>

namespace SDK {
    namespace prefab {
        // Prefab init data wrapper class - 8 bytes
        class C_PrefabInitData {
          public:
            C_PrefabInitData() : m_pConstInitData(nullptr) {}

            void *GetConstInitData() const {
                return m_pConstInitData;
            }

          private:
            void *m_pConstInitData;  // 0x00 - 0x08
        };

        static_assert(sizeof(C_PrefabInitData) == 8, "Wrong size: prefab::C_PrefabInitData");

        // Prefab init data containing GUIDs for deform, fabric, and color
        struct S_PrefabInitData {
            ue::sys::utils::C_GUID m_sDeformGUID;   // 0x00 - 0x10
            ue::sys::utils::C_GUID m_sFabriceGUID;  // 0x10 - 0x20
            ue::sys::utils::C_GUID m_sColorGUID;    // 0x20 - 0x30
        };
    } // namespace prefab
} // namespace SDK
