#pragma once

#include "../ue/sys/utils/c_guid.h"

#include <cstdint>

namespace SDK {
    namespace prefab {
        /**
         * Prefab init data wrapper class.
         * Contains a pointer to the underlying const init data.
         *
         * Class size: 8 bytes
         */
        class C_PrefabInitData {
          public:
            C_PrefabInitData() : m_pConstInitData(nullptr) {}

            /**
             * Get the const init data pointer.
             * @return Pointer to the const init data
             */
            void *GetConstInitData() const {
                return m_pConstInitData;
            }

          private:
            void *m_pConstInitData;  // 0x00 - Pointer to const init data
        };

        static_assert(sizeof(C_PrefabInitData) == 8, "Wrong size: prefab::C_PrefabInitData");

        struct S_PrefabInitData {
            ue::sys::utils::C_GUID m_sDeformGUID;   // 0220 - 0230
            ue::sys::utils::C_GUID m_sFabriceGUID;  // 0230 - 0240
            ue::sys::utils::C_GUID m_sColorGUID;    // 0240 - 0250
        };
    } // namespace prefab
} // namespace SDK
