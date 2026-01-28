#pragma once

#include <cstdint>
#include <vector>

namespace SDK {
    namespace ue::game::translocator {
        struct S_TranslocableObjectDescription {
            int32_t m_LayerMetaDataIndex;             // 0x00 - 0x04
            char _pad04[0x04];                        // 0x04 - 0x08
            std::vector<void *> m_RootMods;           // 0x08 - 0x20
            std::vector<void *> m_ChildrenMods;       // 0x20 - 0x38
            std::vector<void *> m_WorldInteractionMods; // 0x38 - 0x50
        };

        static_assert(sizeof(S_TranslocableObjectDescription) == 0x50, "S_TranslocableObjectDescription size mismatch");

    } // namespace ue::game::translocator
} // namespace SDK
