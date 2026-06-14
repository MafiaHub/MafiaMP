#pragma once

#include <cstdint>

#include "../sys/core/i_component.h"
#include "../sys/utils/c_hash_name.h"
#include "../../entities/c_entity.h"

namespace SDK {
    namespace ue::game {

        // Forward declaration for self-referencing pointer
        class C_EntityWrapperComponent;

        // Component that wraps entity creation and management.
        // Stores entity initialization data, flags, and prefab information.
        // Attached to C_SceneObject via C_ComponentObject system.
        // vtable: 0x144C00C88 | constructor: 0x140f72420
        class C_EntityWrapperComponent : public sys::core::I_Component {
          public:
            uint32_t GetFlags() const {
                return m_iFlags;
            }

            void SetFlags(uint32_t flags) {
                m_iFlags = flags;
            }

            E_EntityType GetGameEntityType() const {
                return static_cast<E_EntityType>(m_eGameEntityType);
            }

            void SetGameEntityType(E_EntityType type) {
                m_eGameEntityType = static_cast<int32_t>(type);
            }

            sys::utils::C_HashName GetModelNameHash() const {
                return sys::utils::C_HashName(m_uModelNameHash);
            }

            void SetModelNameHash(uint64_t hash) {
                m_uModelNameHash = hash;
            }

            const char *GetModelName() const {
                return m_sModelName;
            }

            void *GetBuiltInitProps(size_t *outSize = nullptr) const {
                if (outSize) {
                    *outSize = reinterpret_cast<size_t>(m_pBuiltInitPropsEnd) - reinterpret_cast<size_t>(m_pBuiltInitProps);
                }
                if (m_pBuiltInitPropsEnd == m_pBuiltInitProps) {
                    return nullptr;
                }
                return m_pBuiltInitProps;
            }

            void *GetBuiltPrefabs() const {
                return m_pBuiltPrefabs;
            }

            C_EntityWrapperComponent *GetRelatedWrapper() const {
                return m_pRelatedWrapper;
            }

          protected:
            C_EntityWrapperComponent *m_pRelatedWrapper; // 0x40 - 0x48
            uint64_t m_uModelNameHash;                   // 0x48 - 0x50
            uint32_t m_iFlags;                           // 0x50 - 0x54
            int32_t m_eGameEntityType;                   // 0x54 - 0x58
            uint8_t m_bUnk58;                            // 0x58 - 0x59
            uint8_t m_bDataItemTypeMapInit;              // 0x59 - 0x5A
            char _pad5A[0x06];                           // 0x5A - 0x60
            char *m_sModelName;                          // 0x60 - 0x68
            void *m_pBuiltInitProps;                     // 0x68 - 0x70
            void *m_pBuiltInitPropsEnd;                  // 0x70 - 0x78
            void *m_pUnk78;                              // 0x78 - 0x80
            void *m_pBuiltPrefabs;                       // 0x80 - 0x88
            void *m_pBuiltPrefabsList;                   // 0x88 - 0x90
        };

        static_assert(sizeof(C_EntityWrapperComponent) == 0x90, "C_EntityWrapperComponent size mismatch");

    } // namespace ue::game
} // namespace SDK
