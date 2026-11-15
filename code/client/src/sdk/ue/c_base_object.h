#pragma once

namespace SDK {
    namespace ue {
        // Root of the engine's reflected classes.
        class C_BaseObject {
          public:
            virtual ~C_BaseObject() = default;
            // TODO: GetClassTypeInfo and GetTypeInfo follow the destructor.

          private:
            static void AssertLayout() {
                static_assert(sizeof(C_BaseObject) == 0x08, "C_BaseObject size mismatch");
            }
        };
    } // namespace ue
} // namespace SDK
