#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

#include "shared/modules/human_sync.hpp"

namespace MafiaMP::Shared::Messages::Human {
    class HumanUpdate final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        Modules::HumanSync::UpdateData _updateData{};

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_UPDATE;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->SerializeCompressed(write, _updateData);
        }

        bool Valid() const override {
            // todo
            return true;
        }

        void SetData(const Modules::HumanSync::UpdateData &data) {
            _updateData = data;
        }

        Modules::HumanSync::UpdateData GetData() const {
            return _updateData;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
