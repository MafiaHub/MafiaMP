#pragma once

#include <cstdint>

namespace SDK {
    class I_Human2;

    namespace ue::game::audio::speech {
        class C_SpeechSound {
          public:
            void *m_pVoice;              // 0x00 - 0x08
            uint64_t m_qwUnk08;          // 0x08 - 0x10
            uint32_t m_dwAnimHash;       // 0x10 - 0x14
            uint32_t m_dwSoundHash;      // 0x14 - 0x18
            uint32_t m_dwSpeechType;     // 0x18 - 0x1C
            bool m_bUnk1C;               // 0x1C - 0x1D
            char _pad1D[0x2B];           // 0x1D - 0x48 (includes mutex at 0x1D)
            int m_iState;                // 0x48 - 0x4C
            char _pad4C[0x8];            // 0x4C - 0x54

          public:
            void Play(I_Human2 *human);
            void SetPosition(void *pos, void *dir);
            void Stop(bool finalize);
        };
    } // namespace ue::game::audio::speech
} // namespace SDK
