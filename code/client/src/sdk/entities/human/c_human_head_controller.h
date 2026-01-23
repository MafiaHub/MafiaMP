#pragma once

#include <cstddef>
#include <cstdint>

namespace SDK {
    class I_Human2;

    namespace ue::game::audio::speech {
        class C_SpeechSound;
    }

    namespace ue::game::script {
        class C_SyncObject;
    }

    class C_HumanHeadController {
      public:
        I_Human2 *m_pOwner;                              // 0x00 - 0x08
        uint64_t m_qwSpeechId;                           // 0x08 - 0x10
        ue::game::audio::speech::C_SpeechSound *m_pSpeechSound; // 0x10 - 0x18
        uint32_t m_dwStartTick;                          // 0x18 - 0x1C
        char _pad1C[0x4];                                // 0x1C - 0x20
        ue::game::script::C_SyncObject *m_pSyncObject;   // 0x20 - 0x28
        bool m_bTalkFlag;                                // 0x28 - 0x29
        bool m_bBlockFlag1;                              // 0x29 - 0x2A
        bool m_bBlockFlag2;                              // 0x2A - 0x2B
        bool m_bSilentAnim;                              // 0x2B - 0x2C
        char _pad2C[0x4];                                // 0x2C - 0x30
        uint64_t m_qwAnimHandle;                         // 0x30 - 0x38

      public:
        void ShutUp();
        void Update();

        bool IsTalking() const {
            return m_pSpeechSound != nullptr;
        }
    };

    static_assert(offsetof(C_HumanHeadController, m_pOwner) == 0x00, "C_HumanHeadController::m_pOwner offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_qwSpeechId) == 0x08, "C_HumanHeadController::m_qwSpeechId offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_pSpeechSound) == 0x10, "C_HumanHeadController::m_pSpeechSound offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_dwStartTick) == 0x18, "C_HumanHeadController::m_dwStartTick offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_pSyncObject) == 0x20, "C_HumanHeadController::m_pSyncObject offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_bTalkFlag) == 0x28, "C_HumanHeadController::m_bTalkFlag offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_bBlockFlag1) == 0x29, "C_HumanHeadController::m_bBlockFlag1 offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_bBlockFlag2) == 0x2A, "C_HumanHeadController::m_bBlockFlag2 offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_bSilentAnim) == 0x2B, "C_HumanHeadController::m_bSilentAnim offset mismatch");
    static_assert(offsetof(C_HumanHeadController, m_qwAnimHandle) == 0x30, "C_HumanHeadController::m_qwAnimHandle offset mismatch");
    static_assert(sizeof(C_HumanHeadController) == 0x38, "C_HumanHeadController size mismatch");
} // namespace SDK
