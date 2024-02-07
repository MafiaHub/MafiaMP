#pragma once

#include "../ue/sys/utils/c_guid.h"

namespace SDK{
    namespace prefab {
        struct S_PrefabInitData {
            ue::sys::utils::C_GUID m_sDeformGUID;             // 0220 - ????
            ue::sys::utils::C_GUID m_sFabriceGUID;            // 0230 - ????
            ue::sys::utils::C_GUID m_sColorGUID;              // 0240 - ????
        };
    }
}
