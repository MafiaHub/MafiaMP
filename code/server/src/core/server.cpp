#include "server.h"

#include "shared/messages/human/human_update.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_despawn.h"

#include "shared/modules/human_sync.hpp"
#include "shared/modules/vehicle_sync.hpp"

namespace MafiaMP {

    void Server::PostInit() {
        // register factories
        _humanFactory = std::make_shared<HumanFactory>();

        InitNetworkingMessages();

        // Setup ECS modules
        GetWorldEngine()->GetWorld()->import<Shared::Modules::HumanSync>();
        GetWorldEngine()->GetWorld()->import<Shared::Modules::VehicleSync>();
    }

    void Server::PostUpdate() {}

    void Server::PreShutdown() {}

    void Server::InitNetworkingMessages() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();

        SetOnPlayerConnectCallback([this, net](flecs::entity player, uint64_t guid) {
            this->_humanFactory->SetupServer(net, player);
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
