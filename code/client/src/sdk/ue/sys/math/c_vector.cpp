#include "c_vector.h"

namespace SDK {
    namespace ue::sys::math {
        C_Vector::C_Vector(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        C_Vector C_Vector::applyQuaternion(const C_Quat &q) const {
            auto ix = q.w * x + q.y * z - q.z * y;
            auto iy = q.w * y + q.z * x - q.x * z;
            auto iz = q.w * z + q.x * y - q.y * x;
            auto iw = -q.x * x - q.y * y - q.z * z;

            return {ix * q.w + iw * -q.x + iy * -q.z - iz * -q.y, iy * q.w + iw * -q.y + iz * -q.x - ix * -q.z, iz * q.w + iw * -q.z + ix * -q.y - iy * -q.x};
        }

        C_Vector C_Vector::applyAxisAngle(const C_Vector &axis, float radians) const {
            auto quaternion = C_Quat(axis, radians);

            return applyQuaternion(quaternion);
        }

        C_Vector C_Vector::normalized() const {
            auto length = sqrtf(x * x + y * y + z * z);

            if (length == 0.0f)
                return {0.0f, 0.0f, 0.0f};
            else
                return {x / length, y / length, z / length};
        }

        C_Vector C_Vector::cross(const C_Vector &rhs) const {
            return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
        }

        C_Vector C_Vector::lerp(const C_Vector &rhs, float t) const {
            auto _lerp = [](float a, float b, float t) {
                return a * (1.0f - t) + b * t;
            };
            return {_lerp(x, rhs.x, t), _lerp(y, rhs.y, t), _lerp(z, rhs.z, t)};
        }

        float C_Vector::dist(const C_Vector &pos) const {
            return sqrt((x - pos.x) * (x - pos.x) + (y - pos.y) * (y - pos.y) + (z - pos.z) * (z - pos.z));
        }

        C_Vector C_Vector::operator+(const C_Vector &rhs) const {
            return {x + rhs.x, y + rhs.y, z + rhs.z};
        }

        C_Vector C_Vector::operator-(const C_Vector &rhs) const {
            return {x - rhs.x, y - rhs.y, z - rhs.z};
        }

        C_Vector C_Vector::operator*(float rhs) const {
            return {x * rhs, y * rhs, z * rhs};
        }

        C_Vector C_Vector::operator*(const C_Vector &rhs) const {
            return {x * rhs.x, y * rhs.y, z * rhs.z};
        }

        C_Vector C_Vector::operator/(const C_Vector &rhs) const {
            return {x / rhs.x, y / rhs.y, z / rhs.z};
        }

        void C_Vector::operator*=(const float rhs) {
            x = x * rhs;
            y = y * rhs;
            z = z * rhs;
        };

        inline void C_Vector::print() const {
            printf("[%f, %f, %f]\n", x, y, z);
        }
    } // namespace ue::sys::math
} // namespace SDK