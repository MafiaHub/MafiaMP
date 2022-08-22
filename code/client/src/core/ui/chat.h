#pragma once

#include "utils/safe_win32.h"

#include <fu2/function2.hpp>

#include <string>
#include <vector>

namespace MafiaMP::Core::UI {
    class Chat final {
      public:
        using OnMessageSentProc = fu2::function<void(const std::string &text)>;

        Chat() = default;

        void Update();

        inline void SetOnMessageSentCallback(OnMessageSentProc proc) {
            onMessageSentProc = proc;
        }

        inline void AddMessage(std::string msg) {
            _chatMessages.push_back(msg);
            _newMsgArrived = true;
        }
      private:
        OnMessageSentProc onMessageSentProc {};
        bool _newMsgArrived = false;
        bool _isFocused     = false;
        std::vector<std::string> _chatMessages;
        char _inputText[1024] {};
    };
} // namespace MafiaMP::Core::UI
