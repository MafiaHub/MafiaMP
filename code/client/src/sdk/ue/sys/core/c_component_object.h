#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        /**
         * C_ComponentObject - Base class for scene objects with component system
         * Size: 0x58 (88 bytes)
         *
         * Reversed from constructor at 0x143294a70
         *
         * This class provides a component-based architecture for game objects,
         * allowing dynamic attachment of functionality through components.
         */
        class C_ComponentObject {
          protected:
            void *m_pVtable;                          // 0x00 - 0x08
            void *m_pSetup;                           // 0x08 - 0x10 (C_ComponentObjectSetup*)
            uint32_t m_dwIdOrType;                    // 0x10 - 0x14
            uint8_t _pad14[0x04];                     // 0x14 - 0x18
            void *m_pComponentTypeArrayStart;         // 0x18 - 0x20
            void *m_pComponentTypeArrayEnd;           // 0x20 - 0x28
            void *m_pComponentArrayStart;             // 0x28 - 0x30
            void *m_pComponentArrayEnd;               // 0x30 - 0x38
            void *m_pComponentArrayCapacity;          // 0x38 - 0x40
            void *m_pUnk40;                           // 0x40 - 0x48
            uint64_t m_nNameHash;                     // 0x48 - 0x50
            const char *m_pNameString;                // 0x50 - 0x58
        };

        static_assert(sizeof(C_ComponentObject) == 0x58, "C_ComponentObject size mismatch");
    }; // namespace ue::sys::core
} // namespace SDK
