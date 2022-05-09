#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanSelfUpdate final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        // todo sync data

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_SELF_UPDATE;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            // noop
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
