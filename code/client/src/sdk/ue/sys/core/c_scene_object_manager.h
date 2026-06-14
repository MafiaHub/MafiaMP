#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class I_ResourceList;
        class C_SceneObject;
        class C_SceneObjectRequestCache;
        class C_SceneObjectAsyncRemover;

        class C_SceneObjectManager {
          public:
            I_ResourceList *GetResourceList() const {
                return m_pResourceList;
            }

            C_SceneObjectRequestCache *GetSceneObjectRequestCache() const {
                return m_pRequestCache;
            }

            C_SceneObjectAsyncRemover *GetSceneObjectAsyncRemover() const {
                return m_pAsyncRemover;
            }

          protected:
            char _pad0[0x40];                            // 0x00 - 0x40
            I_ResourceList *m_pResourceList;             // 0x40 - 0x48
            C_SceneObjectRequestCache *m_pRequestCache;  // 0x48 - 0x50
            C_SceneObjectAsyncRemover *m_pAsyncRemover;  // 0x50 - 0x58
        };
    }; // namespace ue::sys::core
} // namespace SDK
