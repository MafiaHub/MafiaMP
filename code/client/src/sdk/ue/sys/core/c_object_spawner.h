#pragma once

#include "../utils/c_hash_name.h"
#include "../../c_cnt_ptr.h"
#include "c_scene.h"

namespace SDK {
    namespace ue::sys::math {
        template <typename T>
        class C_TemplatedVector {

        };
    }
    namespace ue::sys::core {
        class C_PaintLayerType {

        };

        class I_Callback {

        };

        class C_ObjectSpawner {
          public:
            C_ObjectSpawner(ue::C_CntPtr<ue::sys::core::C_Scene>, ue::C_CntPtr<C_PaintLayerType const>, ue::sys::utils::C_HashName, ue::sys::utils::C_HashName, ue::C_CntPtr<I_Callback>, unsigned char, float, ue::sys::math::C_TemplatedVector<int>, unsigned int, bool, float);
        };
    }
}
