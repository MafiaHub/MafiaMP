#pragma once

namespace SDK {
    class C_ItemData {};

    class I_InventoryItem {
      public:
        virtual ~I_InventoryItem() {};
    };

    class C_InventoryItem : public I_InventoryItem {
      public:
        C_ItemData *m_itemData;           // 0008 - 000F
        uint64_t *m_itemStats;            // 0018 - 001F
    };
}
