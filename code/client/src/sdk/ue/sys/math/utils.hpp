#pragma once
#include <algorithm>
#include <chrono>

namespace SDK {
    namespace ue::sys::math {
        template <class T>
        T Lerp(const T &from, const T &to, float fAlpha) {
            return (T)((to - from) * fAlpha + from);
        }

        // Find the relative position of Pos between From and To
        inline const float Unlerp(const double from, const double to, const double pos) {
            // Avoid dividing by 0 (results in INF values)
            if (from == to)
                return 1.0f;

            return static_cast<float>((pos - from) / (to - from));
        }

        inline const float Unlerp(const std::chrono::high_resolution_clock::time_point &from, const std::chrono::high_resolution_clock::time_point &to,
                                  const std::chrono::high_resolution_clock::time_point &pos) {
            float r = std::chrono::duration<float, std::milli>(to - from).count();

            // Avoid dividing by 0 (results in INF values)
            if (r < std::numeric_limits<float>::epsilon())
                return 1.0f;

            return std::chrono::duration<float, std::milli>(pos - from).count() / r;
        }

        // Unlerp avoiding extrapolation
        inline const float UnlerpClamped(const double from, const double to, const double pos) {
            return std::clamp(Unlerp(from, to, pos), 0.0f, 1.0f);
        }
    }; // namespace ue::sys::math
} // namespace SDK