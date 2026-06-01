/*
 * MafiaHub OSS license
 * Copyright (c) 2022, MafiaHub. All rights reserved.
 *
 * This file comes from MafiaHub, hosted at https://github.com/MafiaHub/Framework.
 * See LICENSE file in the source repository for information regarding licensing.
 */

#pragma once

#include <mafianet/BitStream.h>
#include <mafianet/string.h>

#include <string>

namespace MafiaMP::Shared::RPC {
    struct ChatMessage {
        static constexpr const char *kIdentifier = "MafiaMP::ChatMessage";

        MafiaNet::RakString text;

        void FromParameters(const std::string &msg) {
            text = msg.c_str();
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, text);
        }

        bool Valid() const {
            return !text.IsEmpty() && text.GetLength() < 1024;
        }

        std::string GetText() const {
            return text.C_String();
        }
    };
} // namespace MafiaMP::Shared::RPC
