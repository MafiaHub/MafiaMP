#pragma once

#include "c_delegate_manager.h"
#include "c_sound_bank_manager.h"

namespace SDK {
    namespace ue::game::audio {
        class C_AudioDevice {
          public:
            char pad0[0x308];                           // 0000 - 0308
            C_SoundBankManager *m_pSoundBankManager;    // 0308 - 0310
            C_DelegateManager *m_pDelegateManager;      // 0310 - 0318
        };
    }
}
