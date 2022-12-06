#include "chat.h"

#include "core/application.h"
#include "game/helpers/controls.h"

#include <imgui/imgui.h>

namespace MafiaMP::Core::UI {
    void Chat::Update() {
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoScrollbar);
        ImGui::BeginChild("##scrolling", ImVec2(ImGui::GetWindowWidth() * 0.95f, ImGui::GetWindowHeight() * 0.80f));

        if (!_chatMessages.empty()) {
            for (const auto &msg : _chatMessages) {
                ImGui::TextWrapped("%s", msg.c_str());
            }
        }

        if (_newMsgArrived) {
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            _newMsgArrived = false;
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_RETURN) && !_isFocused) {
            _isFocused = true;
            gApplication->LockControls(true);
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        if (_isFocused) {
            ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
            ImGui::SetKeyboardFocusHere(0);
            if (ImGui::InputText("##chatinput", _inputText, sizeof(_inputText), ImGuiInputTextFlags_EnterReturnsTrue)) {
                _isFocused = false;
                if (strlen(_inputText)) {
                    onMessageSentProc(_inputText);
                    strcpy(_inputText, "");
                }

                gApplication->LockControls(false);
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::End();
    }
} // namespace MafiaMP::Core::UI
