#pragma once

#include <integrations/server/instance.h>

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        void InitNetworkingMessages();

      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;
    };
} // namespace MafiaMP
