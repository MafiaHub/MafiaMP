#pragma once

namespace SDK {
    namespace ue::sys::math {
        class C_Vector;
        class C_Quat {
          public:
            C_Quat(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
            C_Quat(const C_Vector &axis, float radians);
            static C_Quat Identity() {
                return C_Quat(0.0f, 0.0f, 0.0f, 1.0f);
            }
            float Norm() const {
                return (x * x + y * y + z * z + w * w);
            }
            float Dot(const C_Quat &other) const {
                return (x * other.x + y * other.y + z * other.z + w * other.w);
            }
            C_Quat Conjugate() const {
                return {-x, -y, -z, w};
            }
            C_Quat Inverse() const;
            C_Quat Normalize() const;
            void SetFromAxisAngle(const C_Vector &axis, float radians);
            void SetDir(const C_Vector &dir, float radians);
            C_Quat Slerp(const C_Quat &to, float t) const;
            C_Quat operator*(const C_Quat &rhs) const;
            C_Quat operator*(float rhs) const;

            void print();

            float x;
            float y;
            float z;
            float w;
        };
    } // namespace ue::sys::math
} // namespace SDK