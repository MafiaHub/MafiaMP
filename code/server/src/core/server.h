#pragma once

#include <integrations/server/instance.h>

#include <scripting/engines/node/engine.h>

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        void InitNetworkingMessages();
        
      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;

        void ModuleRegister(Framework::Scripting::Engines::SDKRegisterWrapper sdk) override;

        void InitRPCs();

        static inline Server *_serverRef = nullptr;
        static inline Framework::Scripting::Engines::Node::Engine *_nodeEngine;
    };
} // namespace MafiaMP
