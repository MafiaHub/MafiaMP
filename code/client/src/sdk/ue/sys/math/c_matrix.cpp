#include "c_matrix.h"
#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::sys::math {
        void C_Matrix::SetDir(C_Vector const &arg1, C_Vector const &arg2) {
            hook::this_call(gPatterns.C_Matrix__SetDir2Addr, this, arg1, arg2);
        }

        void C_Matrix::SetDir(C_Vector const &arg) {
            hook::this_call(gPatterns.C_Matrix__SetDirAddr, this, arg);
        }

        void C_Matrix::SetDir3(C_Vector const &arg) {
            hook::this_call(gPatterns.C_Matrix__SetDir3Addr, this, arg);
        }

        void C_Matrix::SetRot(C_Quat const &rot) {
            hook::this_call(gPatterns.C_Matrix__SetRotAddr, this, rot);
        }

        void C_Matrix::SetRotEuler(float pitch, float yaw, float roll) {
            hook::this_call(gPatterns.C_Matrix__SetRotEulerAddr, this, pitch, yaw, roll);
        }
    }; // namespace ue::sys::math
} // namespace SDK
