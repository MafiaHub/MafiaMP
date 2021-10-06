#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace SDK {
    namespace ue {
        class C_String {
          private:
            uint32_t refCount        = 0;
            int32_t stringLength     = 0;
            int32_t wideStringLength = 0;
            char *stringPtr          = nullptr;

          public:
            const char *c_str() {
                if (!this)
                    return nullptr;

                return *reinterpret_cast<const char **>(this);
            }

            void SetString(const char *str);

            C_String(const char *str) {
                SetString(str);
            }
        };
    }; // namespace ue
} // namespace SDK