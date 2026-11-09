#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::ai {
        class S_HumanAIRequestPlayAnimation;

        // Weak reference the game passes AI requests around with, shared by every weak pointer to the
        // same request.
        class S_HumanAIRequestWeakProxy {
          public:
            S_HumanAIRequestPlayAnimation *GetRequest() const {
                return m_pRequest;
            }
            void Release();

          private:
            uintptr_t m_pVtable;                       // 0x00 - 0x08
            int32_t m_RefCount;                        // 0x08 - 0x0C
            char _pad0[0x4];                           // 0x0C - 0x10
            S_HumanAIRequestPlayAnimation *m_pRequest; // 0x10 - 0x18
        };
        static_assert(sizeof(S_HumanAIRequestWeakProxy) == 0x18, "S_HumanAIRequestWeakProxy size mismatch");

        // The request the play-animation AI action hands the character controller; it activates
        // C_CharacterStateHandlerPlayAnimation. Intrusively counted.
        class S_HumanAIRequestPlayAnimation {
          public:
            // Holds one reference.
            static S_HumanAIRequestPlayAnimation *Create(char const *animationName, bool loop);

            // Holds one reference.
            S_HumanAIRequestWeakProxy *CreateWeakProxy();
            void Release();

          private:
            uintptr_t m_pVtable;                          // 0x00 - 0x08
            uintptr_t m_pRefCountVtable;                  // 0x08 - 0x10
            int32_t m_RefCount;                           // 0x10 - 0x14
            char _pad0[0x4];                              // 0x14 - 0x18
            S_HumanAIRequestWeakProxy *m_pWeakProxy;      // 0x18 - 0x20
            S_HumanAIRequestPlayAnimation *m_pWeakTarget; // 0x20 - 0x28
            char _pad1[0x8];                              // 0x28 - 0x30
            uintptr_t m_pSyncObject;                      // 0x30 - 0x38
            char m_AnimationName[0x80];                   // 0x38 - 0xB8
            char _pad2[0xF0];                             // 0xB8 - 0x1A8
        };
        static_assert(sizeof(S_HumanAIRequestPlayAnimation) == 0x1A8, "S_HumanAIRequestPlayAnimation size mismatch");
    } // namespace ue::game::ai
} // namespace SDK
