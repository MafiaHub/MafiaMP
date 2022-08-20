#pragma once

#include <integrations/server/instance.h>

#include "types/human.hpp"
#include "types/vehicle.hpp"

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        std::shared_ptr<HumanFactory> _humanFactory;
        std::shared_ptr<VehicleFactory> _vehicleFactory;

        void InitNetworkingMessages();

      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;

        std::shared_ptr<HumanFactory> GetHumanFactory() const {
            return _humanFactory;
        }

        std::shared_ptr<VehicleFactory> GetVehicleFactory() const {
            return _vehicleFactory;
        }
    };
} // namespace MafiaMP
