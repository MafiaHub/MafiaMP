#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class I_Component;

        // Component type lookup entry (8 bytes)
        struct S_ComponentTypeEntry {
            uint32_t m_dwTypeID;    // 0x00 - Type hash/ID
            uint32_t m_dwIndex;     // 0x04 - Index into component array (-1 if not present)
        };

        // C_ComponentObjectSetup - Holds component type lookup table
        class C_ComponentObjectSetup {
          public:
            S_ComponentTypeEntry *GetTypeArrayStart() const {
                return m_pTypeArrayStart;
            }

            S_ComponentTypeEntry *GetTypeArrayEnd() const {
                return m_pTypeArrayEnd;
            }

            size_t GetTypeCount() const {
                return m_pTypeArrayEnd - m_pTypeArrayStart;
            }

          protected:
            S_ComponentTypeEntry *m_pTypeArrayStart;  // 0x00 - 0x08
            S_ComponentTypeEntry *m_pTypeArrayEnd;    // 0x08 - 0x10
        };

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
          public:
            C_ComponentObjectSetup *GetSetup() const {
                return m_pSetup;
            }

            I_Component **GetComponentArrayStart() const {
                return m_pComponentArrayStart;
            }

            I_Component **GetComponentArrayEnd() const {
                return m_pComponentArrayEnd;
            }

            size_t GetComponentCount() const {
                if (!m_pComponentArrayStart || !m_pComponentArrayEnd)
                    return 0;
                return m_pComponentArrayEnd - m_pComponentArrayStart;
            }

            I_Component *GetComponentByIndex(size_t index) const {
                if (index >= GetComponentCount())
                    return nullptr;
                return m_pComponentArrayStart[index];
            }

            // Find component by type ID (hash from GetStaticClassTypeInfo()->offset8)
            I_Component *FindComponentByTypeID(uint32_t typeID) const {
                if (!m_pSetup)
                    return nullptr;

                auto typeStart = m_pSetup->GetTypeArrayStart();
                auto typeEnd = m_pSetup->GetTypeArrayEnd();

                for (auto it = typeStart; it != typeEnd; ++it) {
                    if (it->m_dwTypeID == typeID && it->m_dwIndex != 0xFFFFFFFF) {
                        return GetComponentByIndex(it->m_dwIndex);
                    }
                }
                return nullptr;
            }

          protected:
            void *m_pVtable;                          // 0x00 - 0x08
            C_ComponentObjectSetup *m_pSetup;         // 0x08 - 0x10
            uint32_t m_dwIdOrType;                    // 0x10 - 0x14
            uint8_t _pad14[0x04];                     // 0x14 - 0x18
            I_Component **m_pComponentArrayStart;     // 0x18 - 0x20 (array of component pointers)
            I_Component **m_pComponentArrayEnd;       // 0x20 - 0x28
            I_Component **m_pComponentArrayCapacity;  // 0x28 - 0x30
            void *m_pUnk30;                           // 0x30 - 0x38
            void *m_pUnk38;                           // 0x38 - 0x40
            void *m_pUnk40;                           // 0x40 - 0x48
            uint64_t m_nNameHash;                     // 0x48 - 0x50
            const char *m_pNameString;                // 0x50 - 0x58
        };

        static_assert(sizeof(C_ComponentObject) == 0x58, "C_ComponentObject size mismatch");
    }; // namespace ue::sys::core
} // namespace SDK
