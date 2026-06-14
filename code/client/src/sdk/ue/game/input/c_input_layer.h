#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::input {
        class C_InputLayer {
          public:
            virtual ~C_InputLayer() = default;

            void RemoveAllControls();
            void AddDevice(void *pDevice);
            void SetEnabled(bool enabled);
        };
    } // namespace ue::game::input
} // namespace SDK
