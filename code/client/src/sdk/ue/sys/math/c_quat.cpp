#include "c_quat.h"
#include "c_vector.h"
#include "utils.hpp"

#include <cmath>
#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace ue::sys::math {
        C_Quat::C_Quat(float x, float y, float z, float w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        C_Quat::C_Quat(const C_Vector &axis, float radians) {
            SetFromAxisAngle(axis, radians);
        }

        C_Quat C_Quat::Inverse() const {
            float n = Norm();
            return (fabs(n) > 0.0f ? (Conjugate() * (1.0f / n)) : C_Quat(0.0f, 0.0f, 0.0f, 0.0f));
        }

        C_Quat C_Quat::Normalize() const {
            float n = Norm();
            if (n <= 0.0f)
                return Identity();

            float length = sqrtf(n);
            return {x / length, y / length, z / length, w / length};
        }

        void C_Quat::SetFromAxisAngle(const C_Vector &axis, float radians) {
            auto halfDegrees = radians / 2.0f;
            auto s           = sinf(halfDegrees);

            x = axis.x * s;
            y = axis.y * s;
            z = axis.z * s;
            w = cosf(halfDegrees);
        }

        void C_Quat::SetDir(const C_Vector &dir, float radians) {
            hook::this_call(gPatterns.C_Quat__SetDir, this, dir, radians);
        }

        C_Quat C_Quat::Slerp(const C_Quat &to, float t) const {
            C_Quat a = this->Normalize();
            C_Quat b = to.Normalize();

            float dot = a.Dot(b);
            // if the dot product is negative, slerp won't take the shorter path
            // v1 and -v1 are equivalent when the negation is applied to all four components
            // fix by reversing one quaternion
            if (dot < 0.0f) {
                b   = {-b.x, -b.y, -b.z, -b.w};
                dot = -dot;
            }

            const float DOT_THRESHOLD = 0.9995f;
            if (dot > DOT_THRESHOLD) {
                // if the inputs are too close for comfort, linearly interpolate and normalize the result
                C_Quat r;
                r.x = Lerp(a.x, b.x, t);
                r.y = Lerp(a.y, b.y, t);
                r.z = Lerp(a.z, b.z, t);
                r.w = Lerp(a.w, b.w, t);
                return r.Normalize();
            }

            float t_    = 1.0f - t;
            float theta = acos(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
            float sn    = sin(theta);
            float Wa    = sin(t_ * theta) / sn;
            float Wb    = sin(t * theta) / sn;
            C_Quat r;
            r.x = Wa * a.x + Wb * b.x;
            r.y = Wa * a.y + Wb * b.y;
            r.z = Wa * a.z + Wb * b.z;
            r.w = Wa * a.w + Wb * b.w;
            return r.Normalize();
        }

        C_Quat C_Quat::operator*(const C_Quat &rhs) const {
            return {(rhs.w * x) + (rhs.x * w) + (rhs.y * z) - (rhs.z * y), (rhs.w * y) - (rhs.x * z) + (rhs.y * w) + (rhs.z * x),
                    (rhs.w * z) + (rhs.x * y) - (rhs.y * x) + (rhs.z * w), (rhs.w * w) - (rhs.x * x) - (rhs.y * y) - (rhs.z * z)};
        }

        C_Quat C_Quat::operator*(float rhs) const {
            return {rhs * x, rhs * y, rhs * z, rhs * w};
        }

        void C_Quat::print() {
            printf("[%f, %f, %f, %f]\n", x, y, z, w);
        }
    } // namespace ue::sys::math
} // namespace SDK
