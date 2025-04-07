#include "chat.h"

#include <external/imgui/helpers.hpp>

#include "core/application.h"

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI {
    void Chat::OnOpen() {}

    void Chat::OnClose() {}

    void Chat::OnUpdate() {
        bool _wasFocused = _isFocused;

        if (gApplication->GetWebManager()->IsAnyViewFocused()) {
            _isFocused = false;
            return;
        }

        if (gApplication->GetInput()->IsKeyPressed(FW_KEY_RETURN) && !_isFocused) {
            _isFocused = true;
            LockControls(true);
        }

        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        {
            ImGui::BeginChild("##messages", ImVec2(ImGui::GetWindowWidth() * 0.95f, ImGui::GetWindowHeight() * 0.80f), 0, _isFocused ? 0 : ImGuiWindowFlags_NoScrollbar);

            if (!_chatMessages.empty()) {
                for (const auto &msg : _chatMessages) {
                    ImGui::TextWrapped("%s", msg.c_str());
                }
            }

            ImGui::Spacing();

            if (_newMsgArrived) {
                ImGui::SetScrollHereY(1.0f);
                _newMsgArrived = false;
            }

            ImGui::EndChild();
        }

        if (_isFocused) {
            auto inputEditCallback = [&](ImGuiInputTextCallbackData *data) {
                if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory && data->EventKey == ImGuiKey_UpArrow) {
                    if (_history.empty())
                        return 0;
                    if (_historyPos == -1) {
                        _tempInputText = data->Buf;
                    }
                    else if (_historyPos + 1 == _history.size()) {
                        return 0;
                    }
                    ++_historyPos;
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, _history.at(_historyPos).c_str());

                    ImGui::SetKeyboardFocusHere(-1);
                    _isFocused = true;
                }
                else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory && data->EventKey == ImGuiKey_DownArrow) {
                    if (_historyPos > 0) {
                        --_historyPos;
                        data->DeleteChars(0, data->BufTextLen);
                        data->InsertChars(0, _history.at(_historyPos).c_str());
                    }
                    else if (!_tempInputText.empty()) {
                        _historyPos = -1;
                        data->DeleteChars(0, data->BufTextLen);
                        data->InsertChars(0, _tempInputText.c_str());
                        _tempInputText = "";
                    }

                    ImGui::SetKeyboardFocusHere(-1);
                    _isFocused = true;
                }
                return 0;
            };

            ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
            ImGui::SetKeyboardFocusHere(0);
            ImGui::InputText("##input_text", _inputText, sizeof(_inputText), ImGuiInputTextFlags_CallbackHistory, Framework::External::ImGUI::getCallback(inputEditCallback), &inputEditCallback);

            if (_wasFocused && gApplication->GetInput()->IsKeyPressed(FW_KEY_RETURN)) {
                if (strlen(_inputText)) {
                    if (!ProcessBuiltinCommand()) {
                        onMessageSentProc(_inputText);
                    }
                    _history.emplace(_history.begin(), _inputText);
                    strcpy(_inputText, "");
                }

                _historyPos = -1;
                _isFocused  = false;
                LockControls(false);
            }
        }

        ImGui::End();
    }

    void Chat::Clear() {
        _chatMessages.clear();
    }

    bool Chat::ProcessBuiltinCommand() {
        if (std::strcmp(_inputText, "/clear") == 0 || std::strcmp(_inputText, "/cls") == 0) {
            Clear();
            return true;
        }

        return false;
    }
} // namespace MafiaMP::Core::UI
