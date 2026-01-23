#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class C_ComponentObject;

        // Type information structure for components
        // From C_ThisTypeInfo constructor analysis
        struct C_TypeInfo {
            void *m_pVtable;          // 0x00
            uint32_t m_dwTypeID;      // 0x08 - Type hash/ID used for component lookup
            uint32_t m_dwUnk0C;       // 0x0C
            void *m_pUnk10;           // 0x10
            const char *m_pTypeName;  // 0x18 - Type name string
            uint64_t m_nSize;         // 0x20 - Class size in bytes
            // ... more fields

            uint32_t GetTypeID() const {
                return m_dwTypeID;
            }

            const char *GetTypeName() const {
                return m_pTypeName;
            }

            uint64_t GetClassSize() const {
                return m_nSize;
            }
        };

        class I_Component {
          public:
            virtual ~I_Component()                                            = default;
            virtual C_TypeInfo *GetClassTypeInfo()                            = 0;
            virtual C_TypeInfo *GetTypeInfo()                                 = 0;
            virtual C_TypeInfo *GetStaticClassTypeInfo()                      = 0;
            virtual void RegisterForMessages(void *setup)                     = 0;
            virtual void *Unk5()                                              = 0;
            virtual void *Unk6()                                              = 0;
            virtual void *Unk7()                                              = 0;

            C_ComponentObject *GetOwner() const {
                return m_pOwner;
            }

            // Helper to get the type name of this component
            const char *GetComponentTypeName() const {
                // Call virtual GetClassTypeInfo to get runtime type
                auto typeInfo = const_cast<I_Component *>(this)->GetClassTypeInfo();
                if (typeInfo && typeInfo->m_pTypeName) {
                    return typeInfo->m_pTypeName;
                }
                return "Unknown";
            }

            // Helper to get the type ID of this component
            uint32_t GetComponentTypeID() const {
                auto typeInfo = const_cast<I_Component *>(this)->GetClassTypeInfo();
                if (typeInfo) {
                    return typeInfo->m_dwTypeID;
                }
                return 0;
            }

          protected:
            C_ComponentObject *m_pOwner;    // 0x08 - 0x10
            char _pad10[0x30];              // 0x10 - 0x40
        };

        static_assert(sizeof(I_Component) == 0x40, "I_Component size mismatch");

    } // namespace ue::sys::core
} // namespace SDK
