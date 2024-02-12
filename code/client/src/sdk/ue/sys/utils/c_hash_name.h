#pragma once

#include "../../../patterns.h"

#include <stdint.h>

namespace SDK {
    namespace ue::sys::utils {
        class C_HashName {
            uint64_t value = 0;

          public:
            C_HashName() = default;

            C_HashName(uint64_t val): value(val) {}

            static uint64_t ComputeHash(const char *str, bool *pSuccess = nullptr) {
                return hook::call<uint64_t>(gPatterns.C_HashName__ComputeHash, str, pSuccess);
            }

            uint32_t SetName(const char *str) {
                return hook::this_call<uint32_t>(gPatterns.C_HashName__SetName, this, str);
            }

            // Conversion Operator
            operator uint64_t() const {
                return value;
            }
            uint64_t GetValue() const {
                return value;
            }

            // Comparison Operator
            bool operator>(const C_HashName &other) const {
                return value > other.value;
            }
            bool operator>=(const C_HashName &other) const {
                return value >= other.value;
            }

            bool operator<(const C_HashName &other) const {
                return value < other.value;
            }
            bool operator<=(const C_HashName &other) const {
                return value <= other.value;
            }

            bool operator==(const C_HashName &other) const {
                return value == other.value;
            };
            bool operator!=(const C_HashName &other) const {
                return value != other.value;
            };
        };
        static_assert(sizeof(C_HashName) == 8, "C_HashName size mismatch");
    } // namespace ue::sys::utils
} // namespace SDK

namespace std {
    template <>
    struct hash<SDK::ue::sys::utils::C_HashName> {
        size_t operator()(const SDK::ue::sys::utils::C_HashName &hash_name) const {
            return hash_name.GetValue();
        }
    };
} // namespace std
