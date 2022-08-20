#include "server.h"

#include "shared/messages/human/human_update.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_despawn.h"

#include "shared/modules/human_sync.hpp"

namespace MafiaMP {

    void Server::PostInit() {
        InitNetworkingMessages();

        // Setup ECS modules
        GetWorldEngine()->GetWorld()->import<Shared::Modules::HumanSync>();
    }

    void Server::PostUpdate() {}

    void Server::PreShutdown() {}

    void Server::InitNetworkingMessages() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();

        SetOnPlayerConnectCallback([this, net](flecs::entity player, uint64_t guid) {
            auto es = player.get_mut<Framework::World::Modules::Base::Streamable>();

            es->modEvents.spawnProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto frame = e.get<Framework::World::Modules::Base::Frame>();
                Shared::Messages::Human::HumanSpawn humanSpawn;
                humanSpawn.FromParameters(frame->modelHash);
                humanSpawn.SetServerID(e.id());
                net->Send(humanSpawn, guid);
                // todo other stuff
                return true;
            };

            es->modEvents.despawnProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanDespawn humanDespawn;
                humanDespawn.SetServerID(e.id());
                net->Send(humanDespawn, guid);
                return true;
            };

            es->modEvents.selfUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanSelfUpdate humanSelfUpdate;
                humanSelfUpdate.SetServerID(e.id());
                net->Send(humanSelfUpdate, guid);
                return true;
            };

            es->modEvents.updateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto trackingMetadata = e.get<Shared::Modules::HumanSync::UpdateData>();

                Shared::Messages::Human::HumanUpdate humanUpdate{};
                humanUpdate.SetServerID(e.id());
                humanUpdate.SetCharStateHandlerType(trackingMetadata->_charStateHandlerType);
                humanUpdate.SetHealthPercent(trackingMetadata->_healthPercent);
                humanUpdate.SetMoveMode(trackingMetadata->_moveMode);
                humanUpdate.SetSprinting(trackingMetadata->_isSprinting);
                humanUpdate.SetSprintSpeed(trackingMetadata->_sprintSpeed);
                humanUpdate.SetStalking(trackingMetadata->_isStalking);
                net->Send(humanUpdate, guid);
                return true;
            };

            auto frame = player.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelHash = 10505412751276834320;

            player.add<Shared::Modules::HumanSync::UpdateData>();
        });

        SetOnPlayerDisconnectCallback([this](flecs::entity player, uint64_t guid) {
            // todo
        });

        net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
            const auto e = GetWorldEngine()->WrapEntity(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }
            if (!GetWorldEngine()->IsEntityOwner(e, guid.g)) {
                return;
            }

            auto trackingMetadata = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            trackingMetadata->_charStateHandlerType = msg->GetCharStateHandlerType();
            trackingMetadata->_healthPercent        = msg->GetHealthPercent();
            trackingMetadata->_isSprinting          = msg->IsSprinting();
            trackingMetadata->_isStalking           = msg->IsStalking();
            trackingMetadata->_moveMode             = msg->GetMoveMode();
            trackingMetadata->_sprintSpeed          = msg->GetSprintSpeed();

        });
    }
} // namespace MafiaMP
