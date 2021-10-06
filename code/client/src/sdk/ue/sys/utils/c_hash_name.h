#pragma once

#include "../../../patterns.h"

#include <stdint.h>
#include <utils/hooking/hooking.h>

namespace SDK {
    namespace ue::sys::utils {
        class C_HashName {
            uint64_t value = 0;

          public:
            C_HashName() = default;

            C_HashName(uint64_t val): value(val) {}

            static uint64_t ComputeHash(const char *str, bool *pSuccess = nullptr) {
                const auto C_HashName__ComputeHashAddr = reinterpret_cast<uint64_t>(hook::pattern("48 89 74 24 ? 41 56 48 83 EC ? 4C 8B F2 48 8B F1 48 85 C9").get_first());
                return hook::call<uint64_t>(C_HashName__ComputeHashAddr, str, pSuccess);
            }

            uint32_t SetName(const char *str) {
                const auto C_HashName__SetNameAddr = reinterpret_cast<uint64_t>(hook::pattern("44 0F B6 11 4C 8B C2").get_first());
                return hook::this_call<uint32_t>(C_HashName__SetNameAddr, this, str);
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
