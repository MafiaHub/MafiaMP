#pragma once

#include <integrations/server/instance.h>

#include "types/human.hpp"

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        std::shared_ptr<HumanFactory> _humanFactory;

        void InitNetworkingMessages();

      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;
    };
} // namespace MafiaMP
