#include "audio_debug.h"

#include <imgui.h>

#include "sdk/c_game_audio_module.h"

namespace MafiaMP::Core::UI {
    AudioDebug::AudioDebug() {}

    void AudioDebug::Update() {
        const auto pAudioModule = SDK::C_GameAudioModule::GetAudioModule();

        ImGui::Begin("Audio debug", &_visible, ImGuiWindowFlags_AlwaysAutoResize);

        if (pAudioModule) {
            ImGui::Text("Audio Device Pointer : 0x%p\n", pAudioModule->m_pAudioDevice);

            float masterVolume = pAudioModule->m_fMasterVolume;
            if (ImGui::SliderFloat("Master Volume", &masterVolume, 0.0f, 1.0f)) {
                pAudioModule->SetMasterVolume(masterVolume);
            }

            float dialogueVolume = pAudioModule->m_fDialogueVolume;
            if (ImGui::SliderFloat("Dialogue Volume", &dialogueVolume, 0.0f, 1.0f)) {
                pAudioModule->SetDialogueVolume(dialogueVolume);
            }

            float musicVolume = pAudioModule->m_fMusicVolume;
            if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 1.0f)) {
                pAudioModule->SetMusicVolume(musicVolume);
            }

            float sfxVolume = pAudioModule->m_fSfxVolume;
            if (ImGui::SliderFloat("SFX Volume", &sfxVolume, 0.0f, 1.0f)) {
                pAudioModule->SetSfxVolume(sfxVolume);
            }

            float cutsceneVolume = pAudioModule->m_fCutsceneVolume;
            if (ImGui::SliderFloat("Cutscene Volume", &cutsceneVolume, 0.0f, 1.0f)) {
                pAudioModule->SetCutsceneVolume(cutsceneVolume);
            }

            ImGui::Text("Dynamic Range: %i\n", pAudioModule->m_iDynamicRange);
        }

        ImGui::End();
    }
}; // namespace MafiaMP::Core::UI
