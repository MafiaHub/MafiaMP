#pragma once

#include <integrations/server/instance.h>

#include <scripting/server_engine.h>

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        void InitNetworkingMessages();

      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;

        void ModuleRegister(Framework::Scripting::Engine *engine) override;

        void InitRPCs();

        static inline Server *_serverRef = nullptr;
    };
} // namespace MafiaMP
