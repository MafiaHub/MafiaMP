#pragma once

#include "shared/constants.h"
#include "shared/modules/vehicle_sync.hpp"

#include <mafianet/BitStream.h>
#include <mafianet/string.h>
#include <utils/optional.h>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // Sets a subset of properties on the vehicle identified by entityId. Each field is optional;
    // only present fields are applied by the receiver.
    struct VehicleSetProps {
        static constexpr const char *kIdentifier = "MafiaMP::VehicleSetProps";

        uint64_t entityId = 0;
        Framework::Utils::Optional<bool> beaconLightsOn;
        Framework::Utils::Optional<glm::vec4> colorPrimary;
        Framework::Utils::Optional<glm::vec4> colorSecondary;
        Framework::Utils::Optional<float> dirt;
        Framework::Utils::Optional<bool> engineOn;
        Framework::Utils::Optional<float> fuel;
        Framework::Utils::Optional<MafiaNet::RakString> licensePlate;
        Framework::Utils::Optional<Modules::VehicleSync::LockState> lockState;
        Framework::Utils::Optional<bool> radioOn;
        Framework::Utils::Optional<int> radioStationId;
        Framework::Utils::Optional<glm::vec4> rimColor;
        Framework::Utils::Optional<float> rust;
        Framework::Utils::Optional<bool> sirenOn;
        Framework::Utils::Optional<glm::vec4> tireColor;
        Framework::Utils::Optional<glm::vec4> windowTint;

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, entityId);
            beaconLightsOn.Serialize(bs, write);
            colorPrimary.Serialize(bs, write);
            colorSecondary.Serialize(bs, write);
            dirt.Serialize(bs, write);
            engineOn.Serialize(bs, write);
            fuel.Serialize(bs, write);
            licensePlate.Serialize(bs, write);
            lockState.Serialize(bs, write);
            radioOn.Serialize(bs, write);
            radioStationId.Serialize(bs, write);
            rimColor.Serialize(bs, write);
            rust.Serialize(bs, write);
            sirenOn.Serialize(bs, write);
            tireColor.Serialize(bs, write);
            windowTint.Serialize(bs, write);
        }

        bool Valid() const {
            if (licensePlate.HasValue() && licensePlate().GetLength() > Constants::VEHICLE_LICENSE_PLATE_MAX_LENGTH) {
                return false;
            }
            if (radioStationId.HasValue() && (radioStationId() >= -1 && radioStationId() < 4 /* E_RADIO_POLICE */)) {
                return false;
            }
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
