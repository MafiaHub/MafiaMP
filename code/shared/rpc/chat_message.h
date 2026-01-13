
/*
 * MafiaHub OSS license
 * Copyright (c) 2022, MafiaHub. All rights reserved.
 *
 * This file comes from MafiaHub, hosted at https://github.com/MafiaHub/Framework.
 * See LICENSE file in the source repository for information regarding licensing.
 */

#pragma once

#include "shared/messages/messages.h"
#include "src/networking/rpc/rpc.h"

#include <string>

namespace MafiaMP::Shared::RPC {
    class ChatMessage final: public Framework::Networking::RPC::IRPC<ChatMessage> {
      private:
        SLNet::RakString _text;
      public:
        ChatMessage() = default;

        ChatMessage(const std::string &msg)
            : _text(msg.c_str()) {}

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
} // namespace MafiaMP::Shared::RPC
