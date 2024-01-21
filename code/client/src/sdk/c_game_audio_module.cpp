#include "c_game_audio_module.h"

namespace SDK {
    void C_GameAudioModule::SetMasterVolume(float fVolume) {
        hook::call<void>(gPatterns.C_GameAudioModule__SetMasterVolume, this, fVolume);
    }

    void C_GameAudioModule::SetDialogueVolume(float fVolume) {
        hook::call<void>(gPatterns.C_GameAudioModule__SetDialogueVolume, this, fVolume);
    }

    void C_GameAudioModule::SetMusicVolume(float fVolume) {
        hook::call<void>(gPatterns.C_GameAudioModule__SetMusicVolume, this, fVolume);
    }

    void C_GameAudioModule::SetSfxVolume(float fVolume) {
        hook::call<void>(gPatterns.C_GameAudioModule__SetSfxVolume, this, fVolume);
    }

    void C_GameAudioModule::SetCutsceneVolume(float fVolume) {
        hook::call<void>(gPatterns.C_GameAudioModule__SetCutsceneVolume, this, fVolume);
    }

    void C_GameAudioModule::SetDynamicRange(int iRange) {
        hook::call<void>(gPatterns.C_GameAudioModule__SetDynamicRange, this, iRange);
    }
}
