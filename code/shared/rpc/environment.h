#pragma once

#include "shared/messages/messages.h"
#include "src/networking/rpc/rpc.h"

namespace MafiaMP::Shared::RPC {
    class SetEnvironment final: public Framework::Networking::RPC::IRPC<SetEnvironment> {
      private:
        SLNet::RakString _weatherSet;
        float _dayTimeHours;

        public:
        void FromParameters(const std::string &weatherSet, float dayTimeHours) {
              _weatherSet = weatherSet.c_str();
            _dayTimeHours = dayTimeHours;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _weatherSet);
            bs->Serialize(write, _dayTimeHours);
        }

        bool Valid() const override {
            return !_weatherSet.IsEmpty() && _dayTimeHours >= 0.0f && _dayTimeHours <= 24.0f;
        }

        std::string GetWeatherSet() const {
            return _weatherSet.C_String();
        }

        float GetDayTimeHours() const {
            return _dayTimeHours;
        }
    };
}
