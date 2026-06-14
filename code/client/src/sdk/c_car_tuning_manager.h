#pragma once

#include <cstdint>

namespace SDK {
    // Tuning item structure - 208 bytes (0xD0)
    // Each item represents a tuning part (wheels, exhaust, body kit, etc.)
    struct S_TuningItem {
        uint16_t m_uItemId;   // 0x00 - Unique item ID
        uint16_t m_uSlotId;   // 0x02 - Target slot ID (which part of the car)
        char _pad0[0xCC];     // 0x04 - 0xD0 (mesh data, settings, etc.)
    };
    static_assert(sizeof(S_TuningItem) == 0xD0, "S_TuningItem size mismatch");

    // Tuning slot structure - 24 bytes (0x18)
    // Each slot holds a tuning item for a specific part of the car
    struct S_TuningSlot {
        S_TuningItem *m_pCurrentItem;   // 0x00 - Currently installed item pointer
        S_TuningItem *m_pPendingItem;   // 0x08 - Pending item to be installed
        uint16_t m_uSlotId;             // 0x10 - Slot identifier
        char _pad0[0x6];                // 0x12 - 0x18
    };
    static_assert(sizeof(S_TuningSlot) == 0x18, "S_TuningSlot size mismatch");

    // C_CarTuningManager
    // Located at offset 0x1B20 (6944) from C_Car base
    // Manages up to 20 tuning slots for vehicle customization
    class C_CarTuningManager {
      public:
        static constexpr int MAX_TUNING_SLOTS = 20;

        // Sets a tuning item to its designated slot by item ID
        // Returns true if item was found and slot was assigned
        bool SetItemToSlot(uint16_t itemId);

        // Gets the currently installed item ID for a given slot
        // Returns 0 or 0xFFFF if no item is installed
        uint16_t GetInstalledItemId(uint16_t slotId) const;

        // Checks if a specific slot has an item installed
        bool HasItemInSlot(uint16_t slotId) const;

        // Gets the number of active slots
        size_t GetSlotCount() const {
            return (reinterpret_cast<uintptr_t>(m_pSlotsEnd) - reinterpret_cast<uintptr_t>(m_pSlotsBegin)) / sizeof(S_TuningSlot);
        }

        // Gets slot by index
        S_TuningSlot *GetSlot(size_t index) {
            if (index >= GetSlotCount())
                return nullptr;
            return &m_pSlotsBegin[index];
        }

        const S_TuningSlot *GetSlot(size_t index) const {
            if (index >= GetSlotCount())
                return nullptr;
            return &m_pSlotsBegin[index];
        }

        // Gets the number of installed items
        size_t GetInstalledItemCount() const {
            return (reinterpret_cast<uintptr_t>(m_pInstalledItemsEnd) - reinterpret_cast<uintptr_t>(m_pInstalledItemsBegin)) / sizeof(S_TuningItem *);
        }

      private:
        char _pad0[0xF0];                           // 0x000 - 0x0F0
        S_TuningSlot *m_pSlotsBegin;                // 0x0F0 (offset 240)
        S_TuningSlot *m_pSlotsEnd;                  // 0x0F8 (offset 248)
        S_TuningSlot *m_pSlotsCapacity;             // 0x100 (offset 256)
        S_TuningItem **m_pInstalledItemsBegin;      // 0x108 (offset 264)
        S_TuningItem **m_pInstalledItemsEnd;        // 0x110 (offset 272)
        S_TuningItem **m_pInstalledItemsCapacity;   // 0x118 (offset 280)
    };
} // namespace SDK
