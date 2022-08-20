#pragma once

#include "networking/network_peer.h"
#include "world/modules/base.hpp"

#include "shared/messages/human/human_spawn.h"
#include "shared/messages/human/human_despawn.h"
#include "shared/messages/human/human_update.h"
#include "shared/messages/human/human_self_update.h"
#include "shared/modules/human_sync.hpp"

#include <flecs/flecs.h>

namespace MafiaMP {
    class HumanFactory {
      private:
        inline void SetupDefaults(flecs::entity e) {
            auto frame = e.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelHash = 10505412751276834320; /* TODO */

            e.add<Shared::Modules::HumanSync::UpdateData>();
        }

      public:
        inline void SetupServer(Framework::Networking::NetworkServer *net, flecs::entity e) {
            SetupDefaults(e);

            auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();

            es->modEvents.spawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto frame = e.get<Framework::World::Modules::Base::Frame>();
                Shared::Messages::Human::HumanSpawn humanSpawn;
                humanSpawn.FromParameters(frame->modelHash);
                humanSpawn.SetServerID(e.id());
                net->Send(humanSpawn, guid);
                // todo other stuff
                return true;
            };

            es->modEvents.despawnProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanDespawn humanDespawn;
                humanDespawn.SetServerID(e.id());
                net->Send(humanDespawn, guid);
                return true;
            };

            es->modEvents.selfUpdateProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanSelfUpdate humanSelfUpdate;
                humanSelfUpdate.SetServerID(e.id());
                net->Send(humanSelfUpdate, guid);
                return true;
            };

            es->modEvents.updateProc = [net](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
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
        }
    };
} // namespace MafiaMP
