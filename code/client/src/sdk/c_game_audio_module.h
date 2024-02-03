#pragma once

#include "patterns.h"

#include "ue/game/audio/c_audio_device.h"

namespace SDK {
    class C_GameAudioModule {
      public:
        char pad0[0x18];                                // 0000 - 0018
        ue::game::audio::C_AudioDevice* m_pAudioDevice; // 0018 - 0020
        char pad1[0x88];                                // 0020 - 00A8
        float m_fMasterVolume;                          // 00A8 - 00AC
        float m_fDialogueVolume;                        // 00AC - 00B0
        float m_fMusicVolume;                           // 00B0 - 00B4
        float m_fSfxVolume;                             // 00B4 - 00B8
        float m_fCutsceneVolume;                        // 00B8 - 00BC
        int m_iDynamicRange;                            // 00BC - 00C0

      public:
          void SetMasterVolume(float);
          void SetDialogueVolume(float);
          void SetMusicVolume(float);
          void SetSfxVolume(float);
          void SetCutsceneVolume(float);
          void SetDynamicRange(int);

        static C_GameAudioModule *GetAudioModule() {
            return hook::call<C_GameAudioModule *>(gPatterns.C_GameAudioModule__GetAudioModule);
        }
    };
}
