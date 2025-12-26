#pragma once

#include "shared/constants.h"
#include "shared/modules/vehicle_sync.hpp"
#include "src/networking/rpc/game_rpc.h"
#include "utils/optional.h"

namespace MafiaMP::Shared::RPC {
    class VehicleSetProps final: public Framework::Networking::RPC::IGameRPC<VehicleSetProps> {
      public:
        Framework::Utils::Optional<bool> beaconLightsOn;
        Framework::Utils::Optional<glm::vec4> colorPrimary;
        Framework::Utils::Optional<glm::vec4> colorSecondary;
        Framework::Utils::Optional<float> dirt;
        Framework::Utils::Optional<bool> engineOn;
        Framework::Utils::Optional<float> fuel;
        Framework::Utils::Optional<SLNet::RakString> licensePlate;
        Framework::Utils::Optional<Modules::VehicleSync::LockState> lockState;
        Framework::Utils::Optional<bool> radioOn;
        Framework::Utils::Optional<int> radioStationId;
        Framework::Utils::Optional<glm::vec4> rimColor;
        Framework::Utils::Optional<float> rust;
        Framework::Utils::Optional<bool> sirenOn;
        Framework::Utils::Optional<glm::vec4> tireColor;
        Framework::Utils::Optional<glm::vec4> windowTint;

        VehicleSetProps() = default;

        VehicleSetProps(const VehicleSetProps &props)
            : beaconLightsOn(props.beaconLightsOn)
            , colorPrimary(props.colorPrimary)
            , colorSecondary(props.colorSecondary)
            , dirt(props.dirt)
            , engineOn(props.engineOn)
            , fuel(props.fuel)
            , licensePlate(props.licensePlate)
            , lockState(props.lockState)
            , radioOn(props.radioOn)
            , radioStationId(props.radioStationId)
            , rimColor(props.rimColor)
            , rust(props.rust)
            , sirenOn(props.sirenOn)
            , tireColor(props.tireColor)
            , windowTint(props.windowTint) {}

        void Serialize(SLNet::BitStream *bs, bool write) override {
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

        bool Valid() const override {
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
