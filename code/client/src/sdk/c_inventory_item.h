#pragma once

namespace SDK {
    class I_ItemData {};
    // TODO: check if it's C_ItemBase or C_ItemDataInventoryItem
    class C_ItemData: public I_ItemData {
      public:
        uint64_t m_someSize; // 0008 - 0010
        uint32_t m_itemId;   // 000C - 0010
        uint64_t m_unknown;  // 0010 - 0018
    };

    class I_InventoryItem {
      public:
        virtual ~I_InventoryItem() {};
    };

    class C_InventoryItem : public I_InventoryItem {
      public:
        C_ItemData *m_itemData;             // 0008 - 0010
        bool m_dropable;                    // 0010 - 0011
        bool m_changed;                     // 0011 - 0012
        uint16_t _padding;                  // 0012 - 0014
    };

    // static_assert(sizeof(C_InventoryItem) == 0x14, "Wrong size: C_InventoryItem");
}
