#include "application.h"

#include <logging/logger.h>

#include "states/initialize.h"
#include "states/menu.h"
#include "states/session_connection.h"
#include "states/session_connected.h"
#include "states/session_disconnection.h"
#include "states/session_offline_debug.h"
#include "states/shutdown.h"
#include "states/states.h"

#include "../sdk/c_game.h"
#include "../sdk/entities/c_car.h"
#include "../sdk/entities/c_player_2.h"
#include "../sdk/entities/c_vehicle.h"
#include "../sdk/entities/human/c_human_weapon_controller.h"

#include "../shared/messages/human/human_spawn.h"
#include "../shared/messages/human/human_despawn.h"
#include "../shared/messages/human/human_update.h"
#include "../shared/messages/human/human_self_update.h"
#include "../shared/messages/human/human_client_update.h"

#include "../game/helpers/controls.h"
#include "external/imgui/widgets/corner_text.h"

#include <utils/version.h>
#include "../shared/version.h"

#include "modules/human.hpp"

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    bool Application::PostInit() {
        // Create the state machine and initialize
        _stateMachine  = new Framework::Utils::States::Machine;
        _stateMachine->RegisterState<States::InitializeState>();
        _stateMachine->RegisterState<States::InMenuState>();
        _stateMachine->RegisterState<States::SessionConnectionState>();
        _stateMachine->RegisterState<States::SessionConnectedState>();
        _stateMachine->RegisterState<States::SessionDisconnectionState>();
        _stateMachine->RegisterState<States::SessionOfflineDebugState>();
        _stateMachine->RegisterState<States::ShutdownState>();

        // Create the entity factory
        _entityFactory = new Game::Streaming::EntityFactory;

        // Register other things
        InitNetworkingMessages();

        // This must always be the last call
        _stateMachine->RequestNextState(States::StateIds::Initialize);

        // Register client modules
        GetWorldEngine()->GetWorld()->import<Modules::Human>();

        GetWorldEngine()->SetOnEntityDestroyCallback([](flecs::entity e) {
            if (e.get<Core::Modules::Human::Tracking>() != nullptr) {
                Core::Modules::Human::RemoveHuman(e);
            }

            return true;
        });
        return true;
    }

    bool Application::PreShutdown() {
        if (_entityFactory) {
            _entityFactory->ReturnAll();
        }
        return true;
    }

    void Application::PostUpdate() {
        if (_stateMachine) {
            _stateMachine->Update();
        }

        if (_entityFactory) {
            _entityFactory->Update();
        }

#if 1
        Core::gApplication->GetImGUI()->PushWidget([&]() {
            using namespace Framework::External::ImGUI::Widgets;
            const auto networkClient = Core::gApplication->GetNetworkingEngine()->GetNetworkClient();
            const auto connState = networkClient->GetConnectionState();
            const auto ping      = networkClient->GetPing();

            constexpr char *connStateNames[3] = {
                "Connecting",
                "Online",
                "Offline"
            };

            // versioning
            DrawCornerText(CORNER_RIGHT_BOTTOM, "Mafia: Multiplayer");
            DrawCornerText(CORNER_RIGHT_BOTTOM, fmt::format("Framework version: {} ({})", Framework::Utils::Version::rel, Framework::Utils::Version::git));
            DrawCornerText(CORNER_RIGHT_BOTTOM, fmt::format("MafiaMP version: {} ({})", MafiaMP::Version::rel, MafiaMP::Version::git));

            // connection details
            DrawCornerText(CORNER_LEFT_BOTTOM, fmt::format("Connection: {}", connStateNames[connState]));
            DrawCornerText(CORNER_LEFT_BOTTOM, fmt::format("Ping: {}", ping));

            // controls
            DrawCornerText(CORNER_LEFT_TOP, "Controls:");
            DrawCornerText(CORNER_LEFT_TOP, "F1 - spawn car");
            DrawCornerText(CORNER_LEFT_TOP, "F3 - despawn all");
            DrawCornerText(CORNER_LEFT_TOP, "F5 - disconnect from server");
        });
#endif
    }

    void Application::InitNetworkingMessages() {
        SetOnConnectionFinalizedCallback([this](flecs::entity newPlayer) {
            _stateMachine->RequestNextState(States::StateIds::SessionConnected);
            auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();

            _localPlayer = newPlayer;

            auto trackingData = _localPlayer.get_mut<Core::Modules::Human::Tracking>();
            trackingData->human = localPlayer;
            trackingData->info = nullptr;

            _localPlayer.add<Core::Modules::Human::LocalPlayer>();

            const auto es = _localPlayer.get_mut<Framework::World::Modules::Base::Streamable>();
            es->modEvents.clientUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanClientUpdate humanUpdate;
                humanUpdate.FromParameters(e.id());
                // set up sync data
                peer->Send(humanUpdate, guid);
                return true;
            };
        });

        SetOnConnectionClosedCallback([this]() {
            _stateMachine->RequestNextState(States::StateIds::SessionDisconnection);
        });

        // TEMP hook up human events
        const auto net = GetNetworkingEngine()->GetNetworkClient();

        net->RegisterMessage<Shared::Messages::Human::HumanSpawn>(Shared::Messages::ModMessages::MOD_HUMAN_SPAWN, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSpawn *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // call GetPlayerFactory()->SetupClient(e, guid)
            GetPlayerFactory()->SetupClient(e, guid.g);

            // setup tracking info
            Core::Modules::Human::CreateHuman(e, msg->GetSpawnProfile());

            // set up client updates
            const auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();
            es->modEvents.clientUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanClientUpdate humanUpdate;
                humanUpdate.FromParameters(e.id());
                // set up sync data
                peer->Send(humanUpdate, guid);
                return true;
            };
        });
        net->RegisterMessage<Shared::Messages::Human::HumanDespawn>(Shared::Messages::ModMessages::MOD_HUMAN_DESPAWN, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanDespawn *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            Core::Modules::Human::RemoveHuman(e);
        });
        net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            Core::Modules::Human::UpdateHuman(e);
        });
        net->RegisterMessage<Shared::Messages::Human::HumanSelfUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_SELF_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSelfUpdate *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            const auto trackingData = e.get<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            // update actor data
        });
    }
} // namespace MafiaMP::Core
