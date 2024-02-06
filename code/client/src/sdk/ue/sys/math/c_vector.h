#pragma once

namespace SDK {
    namespace ue::sys::math {
        class C_Quat;
        class C_Vector2 {
          public:
            float x;
            float y;
        };

        class C_Vector4 {
          public:
            float r;
            float g;
            float b;
            float a;
        };

        class C_Vector {
          public:
            C_Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f);
            C_Vector applyQuaternion(const C_Quat &q) const;
            C_Vector applyAxisAngle(const C_Vector &axis, float radians) const;
            C_Vector normalized() const;
            C_Vector cross(const C_Vector &rhs) const;
            C_Vector lerp(const C_Vector &rhs, float t) const;
            float dist(const C_Vector &pos) const;
            C_Vector operator+(const C_Vector &rhs) const;
            C_Vector operator-(const C_Vector &rhs) const;
            C_Vector operator*(const C_Vector &rhs) const;
            C_Vector operator*(float rhs) const;
            C_Vector operator/(const C_Vector &rhs) const;
            void operator*=(const float rhs);

            inline void print() const;

            float x;
            float y;
            float z;
        };
    } // namespace ue::sys::math
} // namespace SDK
