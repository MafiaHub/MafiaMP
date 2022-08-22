
#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

#include <string>

namespace MafiaMP::Shared::Messages::Misc {
    class ChatMessage final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        SLNet::RakString _text;
      public:
        uint8_t GetMessageID() const override {
            return MOD_CHAT_MESSAGE;
        }

        void FromParameters(const std::string &msg) {
            _text = msg.c_str();
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _text);
        }

        bool Valid() const override {
            return !_text.IsEmpty() && _text.GetLength() < 1024;
        }

        std::string GetText() const {
            return _text.C_String();
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
