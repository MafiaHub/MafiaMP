#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::sys::core {
        class C_SceneObject;

        class C_SceneObjectAsyncRemover {
          protected:
            char m_Mutex[0x30];           // 0x00 - 0x30
            void *m_pQueueStart;          // 0x30 - 0x38
            void *m_pQueueEnd;            // 0x38 - 0x40
            void *m_pQueueCapacity;       // 0x40 - 0x48
            double m_fLastProcessTime;    // 0x48 - 0x50
        };
    }; // namespace ue::sys::core
} // namespace SDK
