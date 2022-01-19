#include "server.h"

#include "shared/messages/human/human_client_update.h"
#include "shared/messages/human/human_update.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_despawn.h"

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
                const auto frame = e.get<Framework::World::Modules::Base::Frame>();
                Shared::Messages::Human::HumanSpawn humanSpawn;
                humanSpawn.FromParameters(e.id(), frame->modelHash);
                net->Send(humanSpawn, guid);
                // todo other stuff
                return true;
            };

            es->modEvents.despawnProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanDespawn humanDespawn;
                humanDespawn.FromParameters(e.id());
                net->Send(humanDespawn, guid);
                return true;
            };

            es->modEvents.selfUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanSelfUpdate humanSelfUpdate;
                humanSelfUpdate.FromParameters(e.id());
                net->Send(humanSelfUpdate, guid);
                return true;
            };

            es->modEvents.updateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanUpdate humanUpdate;
                humanUpdate.FromParameters(e.id());
                net->Send(humanUpdate, guid);
                return true;
            };

            auto frame = player.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelHash = 5566874084318867535;
        });

        SetOnPlayerDisconnectedCallback([this](flecs::entity player) {
            // todo
        });

        net->RegisterMessage<Shared::Messages::Human::HumanClientUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_CLIENT_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanClientUpdate *msg) {
            const auto e = GetWorldEngine()->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }
            if (!GetWorldEngine()->IsEntityOwner(e, guid.g)) {
                return;
            }

            // update actor data
        });
    }
} // namespace MafiaMP
