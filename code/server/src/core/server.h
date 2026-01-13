#pragma once

#include <integrations/server/instance.h>

#include <scripting/server_engine.h>

#include <RakNetTypes.h>

namespace MafiaMP::Shared::RPC {
    class ChatMessage;
    class HumanChangeSkin;
} // namespace MafiaMP::Shared::RPC

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      private:
        void InitNetworkingMessages();

        void OnChatMessage(SLNet::RakNetGUID guid, Shared::RPC::ChatMessage *msg);
        void OnHumanChangeSkin(SLNet::RakNetGUID guid, Shared::RPC::HumanChangeSkin *msg);

      public:
        void PostInit() override;

        void PostUpdate() override;

        void PreShutdown() override;

        void ModuleRegister(Framework::Scripting::Engine *engine) override;

        void InitRPCs();

        static inline Server *_serverRef = nullptr;
    };
} // namespace MafiaMP
