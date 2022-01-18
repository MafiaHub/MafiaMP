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

        SetOnPlayerConnectedCallback([this, net](flecs::entity player) {
            auto es = player.get_mut<Framework::World::Modules::Base::Streamable>();

            es->modEvents.spawnProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                // todo
                return true;
            };

            es->modEvents.despawnProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                // todo
                return true;
            };

            es->modEvents.selfUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                // todo
                return true;
            };

            es->modEvents.updateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                // todo
                return true;
            };
        });

        SetOnPlayerDisconnectedCallback([this](flecs::entity player) {
            // todo
        });

        net->RegisterMessage<Shared::Messages::Human::HumanClientUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_CLIENT_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanClientUpdate *msg) {
            const auto e = GetWorldEngine()->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // update actor data
        });
    }
} // namespace MafiaMP
