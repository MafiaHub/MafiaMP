#pragma once

#include <integrations/server/instance.h>

#include <mafianet/types.h>

#include <cstdint>
#include <string>
#include <vector>

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

        void OnPlayerConnect(const Framework::Integrations::Server::PlayerConnectionData &info) override;
        void OnPlayerDisconnect(MafiaNet::PeerGuid guid) override;
        void OnChatMessage(uint64_t senderId, const std::string &text) override;
        void OnChatCommand(uint64_t senderId, const std::string &text, const std::string &command, const std::vector<std::string> &args) override;

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
