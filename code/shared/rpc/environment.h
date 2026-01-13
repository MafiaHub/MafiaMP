#pragma once

#include "shared/messages/messages.h"
#include "src/networking/rpc/rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class SetEnvironment final: public Framework::Networking::RPC::IRPC<SetEnvironment> {
      private:
        Framework::Utils::Optional<SLNet::RakString> _weatherSet{};
        Framework::Utils::Optional<float> _dayTimeHours{};

        public:
        SetEnvironment() = default;

        SetEnvironment(const Framework::Utils::Optional<SLNet::RakString> &weatherSet, Framework::Utils::Optional<float> dayTimeHours = {})
            : _weatherSet(weatherSet)
            , _dayTimeHours(dayTimeHours) {}

        void Serialize(SLNet::BitStream *bs, bool write) override {
            _weatherSet.Serialize(bs, write);
            _dayTimeHours.Serialize(bs, write);
        }

        bool Valid() const override {
            if (_weatherSet.HasValue() && _weatherSet().IsEmpty()) {
                return false;
            }
            if (_dayTimeHours.HasValue() && (_dayTimeHours() < 0.0f || _dayTimeHours() > 24.0f)) {
                return false;
            }
            return true;
        }

        Framework::Utils::Optional<SLNet::RakString> GetWeatherSet() const {
            return _weatherSet;
        }

        Framework::Utils::Optional<float> GetDayTimeHours() const {
            return _dayTimeHours;
        }
    };
}
