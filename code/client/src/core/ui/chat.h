#pragma once

#include "ui_base.h"

#include <fu2/function2.hpp>
#include <string>
#include <vector>

namespace MafiaMP::Core::UI {
    class Chat final: public UIBase {
      public:
        using OnMessageSentProc = fu2::function<void(const std::string &text)>;

      private:
        OnMessageSentProc onMessageSentProc {};

        bool _newMsgArrived = false;

        bool _isFocused = false;

        std::vector<std::string> _chatMessages;

        char _inputText[1024] {};

        std::vector<std::string> _history;

        std::string _tempInputText;

        int _historyPos = -1;

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        Chat(): UIBase() {};

        inline void SetOnMessageSentCallback(OnMessageSentProc proc) {
            onMessageSentProc = proc;
        }

        inline void AddMessage(std::string msg) {
            _chatMessages.push_back(msg);
            _newMsgArrived = true;
        }
    };
} // namespace MafiaMP::Core::UI
