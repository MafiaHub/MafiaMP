#include "c_car_tuning_manager.h"
#include "../patterns.h"

namespace SDK {
    bool C_CarTuningManager::SetItemToSlot(uint16_t itemId) {
        return hook::this_call<bool>(gPatterns.C_CarTuningManager__SetItemToSlot, this, itemId);
    }

    uint16_t C_CarTuningManager::GetInstalledItemId(uint16_t slotId) const {
        size_t count = GetSlotCount();
        for (size_t i = 0; i < count; i++) {
            const S_TuningSlot *slot = GetSlot(i);
            if (slot && slot->m_uSlotId == slotId && slot->m_pCurrentItem) {
                return slot->m_pCurrentItem->m_uItemId;
            }
        }
        return 0xFFFF;
    }

    bool C_CarTuningManager::HasItemInSlot(uint16_t slotId) const {
        size_t count = GetSlotCount();
        for (size_t i = 0; i < count; i++) {
            const S_TuningSlot *slot = GetSlot(i);
            if (slot && slot->m_uSlotId == slotId) {
                return slot->m_pCurrentItem != nullptr;
            }
        }
        return false;
    }
} // namespace SDK
