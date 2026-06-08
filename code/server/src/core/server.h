#pragma once

#include <integrations/server/instance.h>

#include <mafianet/types.h>

#include <string>

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      public:
        // Global world state pushed to clients via the SetEnvironment RPC.
        struct Environment {
            std::string weatherSet = "_default_game";
            float dayTimeHours     = 11.0f;
        };

        void PostInit() override;
        void PostUpdate() override;
        void PreShutdown() override;
        void ModuleRegister(Framework::Scripting::Engine *engine) override;

        const Environment &GetEnvironment() const {
            return _environment;
        }
        void SetWeatherSet(const std::string &weatherSet);
        void SetDayTimeHours(float dayTimeHours);
        // Send the current environment to one system (UNASSIGNED = broadcast to all).
        void SendEnvironment(MafiaNet::RakNetGUID target, bool broadcast) const;

        static inline Server *_serverRef = nullptr;

      private:
        void InitNetworkingMessages();
        void InitRPCs();

        Environment _environment {};
    };
} // namespace MafiaMP
