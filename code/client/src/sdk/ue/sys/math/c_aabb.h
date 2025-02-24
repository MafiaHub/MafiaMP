#pragma once

#include "c_vector.h"

namespace SDK {
    namespace ue::sys::math {
        // Axis-Aligned Bounding Box
        class C_AABB {
          public:
            C_Vector m_vMin;            // 0000 - 000C
            C_Vector m_vMax;            // 000C - 0018

            C_AABB(const C_Vector& vec) {
                m_vMin.x = -vec.x;
                m_vMin.y = -vec.y;
                m_vMin.z = -vec.z;
                m_vMax.x = vec.x;
                m_vMax.y = vec.y;
                m_vMax.z = vec.z;
            }
        };
    }
}
