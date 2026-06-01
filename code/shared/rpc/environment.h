#pragma once

#include <mafianet/BitStream.h>
#include <mafianet/string.h>
#include <utils/optional.h>

namespace MafiaMP::Shared::RPC {
    struct SetEnvironment {
        static constexpr const char *kIdentifier = "MafiaMP::SetEnvironment";

        Framework::Utils::Optional<MafiaNet::RakString> weatherSet {};
        Framework::Utils::Optional<float> dayTimeHours {};

        void FromParameters(const Framework::Utils::Optional<MafiaNet::RakString> &weatherSet_ = {}, Framework::Utils::Optional<float> dayTimeHours_ = {}) {
            weatherSet   = weatherSet_;
            dayTimeHours = dayTimeHours_;
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            weatherSet.Serialize(bs, write);
            dayTimeHours.Serialize(bs, write);
        }

        bool Valid() const {
            if (weatherSet.HasValue() && weatherSet().IsEmpty()) {
                return false;
            }
            if (dayTimeHours.HasValue() && (dayTimeHours() < 0.0f || dayTimeHours() > 24.0f)) {
                return false;
            }
            return true;
        }

        Framework::Utils::Optional<MafiaNet::RakString> GetWeatherSet() const {
            return weatherSet;
        }

        Framework::Utils::Optional<float> GetDayTimeHours() const {
            return dayTimeHours;
        }
    };
} // namespace MafiaMP::Shared::RPC
