#pragma once

#include "sdk/patterns.h"

namespace SDK {
    namespace script {
        static int32_t GetWeaponIdByName(const char* weaponName) {
            return hook::call<int32_t>(gPatterns.Script__GetWeaponIdByName, weaponName);
        }
    }
}
