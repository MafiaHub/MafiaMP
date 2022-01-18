#include "server.h"

#include "shared/messages/human/human_client_update.h"

namespace MafiaMP {

    void Server::PostInit() {
        InitNetworkingMessages();

        // import modules
    }

    void Server::PostUpdate() {}

    void Server::PreShutdown() {}

    void Server::InitNetworkingMessages() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();

        net->RegisterMessage<Shared::Messages::Human::HumanClientUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_CLIENT_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanClientUpdate *msg) {
            const auto e = GetWorldEngine()->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // update actor data
        });
    }
} // namespace MafiaMP
