#include "audio_debug.h"
#include <external/imgui/wrapper.h>
#include <imgui.h>

#include "sdk/c_game_audio_module.h"

namespace MafiaMP::Core::UI {
    AudioDebug::AudioDebug() {}

    void AudioDebug::Update() {
        const auto pAudioModule = SDK::C_GameAudioModule::GetAudioModule();

        ImGui::Begin("Audio debug", &_visible, ImGuiWindowFlags_AlwaysAutoResize);

        if (pAudioModule) {
            ImGui::Text("Audio Device Pointer : 0x%p\n", pAudioModule->m_pAudioDevice);
            ImGui::Text("Master Volume: %f\n", pAudioModule->m_fMasterVolume);
            ImGui::Text("Dialogue Volume: %f\n", pAudioModule->m_fDialogueVolume);
            ImGui::Text("Music Volume: %f\n", pAudioModule->m_fMusicVolume);
            ImGui::Text("SFX Volume: %f\n", pAudioModule->m_fSfxVolume);
            ImGui::Text("Cutscene Volume: %f\n", pAudioModule->m_fCutsceneVolume);
            ImGui::Text("Dynamic Range: %i\n", pAudioModule->m_iDynamicRange);
        }
        else {
            ImGui::Text("You're not in a vehicle!");
        }

        ImGui::End();
    }
}; // namespace MafiaMP::Core::UI
