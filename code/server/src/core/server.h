#pragma once

#include <integrations/server/instance.h>

#include <scripting/server_engine.h>

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        static inline Framework::Scripting::ServerEngine *_scriptingEngine;
        void InitNetworkingMessages();

      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;

        void ModuleRegister(Framework::Scripting::ServerEngine *engine) override;

        void InitRPCs();

        static inline Server *_serverRef = nullptr;

        static Framework::Scripting::ServerEngine *GetScriptingEngine() {
            return _scriptingEngine;
        }
    };
} // namespace MafiaMP
