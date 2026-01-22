#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::input {
        class C_InputLayer;

        enum class E_InputLayerType : uint32_t {};

        class C_InputSet {
          public:
            virtual ~C_InputSet() = default;

            C_InputLayer *CreateInputLayer(float priority, E_InputLayerType type, bool enabled);
        };
    } // namespace ue::game::input
} // namespace SDK
