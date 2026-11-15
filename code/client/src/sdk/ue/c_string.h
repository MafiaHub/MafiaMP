#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace SDK {
    namespace ue {
        // A reference-counted string: one pointer to the characters, with the game's allocation
        // header (reference count, lengths) in front of them. An empty string holds no data.
        class C_String {
          public:
            C_String() = default;

            C_String(const char *str) {
                SetString(str);
            }

            const char *c_str() const {
                if (!this)
                    return nullptr;

                return m_pData;
            }

            void SetString(const char *str);

          private:
            char *m_pData = nullptr; // 0x00 - 0x08

            static void AssertLayout() {
                static_assert(sizeof(C_String) == 0x08, "C_String size mismatch");
                static_assert(offsetof(C_String, m_pData) == 0x00, "C_String::m_pData offset mismatch");
            }
        };
    }; // namespace ue
} // namespace SDK
