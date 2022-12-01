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

        void BroadcastChatMessage(flecs::entity ent, const std::string &msg);

        void InitRPCs();

        static inline Server *_serverRef = nullptr;
    };
} // namespace MafiaMP
