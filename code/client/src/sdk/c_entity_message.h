#pragma once

namespace SDK {
    class C_EntityMessage {
      public:
        void *m_pUnk;                   // 0x08 - 0x10: Unknown pointer
        int m_iEntityID;                // 0x10 - 0x14: Entity ID
        int m_iMessageType;             // 0x14 - 0x18: Message type identifier
        int m_iTargetEntityID;          // 0x18 - 0x1C: Target entity ID (-1 in constructor)
        bool m_bActive;                 // 0x1C - 0x1D: Active flag (set to 1)
        char pad[0x3];                  // 0x1D - 0x20

      public:
        virtual ~C_EntityMessage() = default;
    };
}
