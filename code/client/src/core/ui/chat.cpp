#include "chat.h"

#include "core/application.h"
#include "game/helpers/controls.h"

#include <imgui/imgui.h>

namespace MafiaMP::Core::UI {
    void Chat::Update() {
        ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

        ImGui::SetWindowSize(ImVec2(400, 300));
        ImGui::SetWindowPos(ImVec2(20, 20));
        ImGui::BeginChild("##scrolling");

        if (!_chatMessages.empty()) {
            for (auto message : _chatMessages) { ImGui::Text("%s", message.c_str()); }
        }

        if (_newMsgArrived) {
            ImGui::SetScrollHereY(1.0f);
            _newMsgArrived = false;
        }

        if (GetAsyncKeyState(VK_RETURN) & 0x1 && !_isFocused) {
            _isFocused = true;
            gApplication->LockControls(true);
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        if (_isFocused) {
            ImGui::SetKeyboardFocusHere(0);
            if (ImGui::InputText("##chatinput", _inputText, sizeof(_inputText), ImGuiInputTextFlags_EnterReturnsTrue)) {
                _isFocused = false;
                if (strlen(_inputText)) {
                    onMessageSentProc(_inputText);
                    strcpy(_inputText, "");
                }

                gApplication->LockControls(false);
                ImGui::SetScrollHereY(1.0f);
            }
        } else {
            ImGui::InputText("##chatinputfake", _inputText, sizeof(_inputText), ImGuiInputTextFlags_ReadOnly);
        }
        ImGui::SetScrollHereY(1.0f);
        ImGui::End();
    }
} // namespace MafiaMP::Core::UI
