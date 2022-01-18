#include "application.h"

#include <logging/logger.h>

#include "states/initialize.h"
#include "states/menu.h"
#include "states/session_connection.h"
#include "states/session_connected.h"
#include "states/session_disconnection.h"
#include "states/shutdown.h"
#include "states/states.h"

#include "../shared/messages/human/human_spawn.h"
#include "../shared/messages/human/human_despawn.h"
#include "../shared/messages/human/human_update.h"
#include "../shared/messages/human/human_self_update.h"

#include "../game/helpers/controls.h"
#include "external/imgui/widgets/corner_text.h"

#include <utils/version.h>
#include "../shared/version.h"

#include "../modules/human.hpp"

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
        _stateMachine->RegisterState<States::ShutdownState>();

        // Create the entity factory
        _entityFactory = new Game::Streaming::EntityFactory;

        // Register other things
        InitNetworkingMessages();

        // This must always be the last call
        _stateMachine->RequestNextState(States::StateIds::Initialize);

        // Register client modules
        GetWorldEngine()->GetWorld()->import<Modules::Human>();
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
        SetOnConnectionFinalizedCallback([this](flecs::entity_t serverID) {
            _stateMachine->RequestNextState(States::StateIds::SessionConnected);

            auto guid = GetNetworkingEngine()->GetNetworkClient()->GetPeer()->GetMyGUID();
            auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();

            const auto localPlayerEntity = GetWorldEngine()->CreateEntity(serverID);
            _localPlayer = GetPlayerFactory()->SetupClient(localPlayerEntity, guid.g);

            auto trackingData = _localPlayer.get_mut<Core::Modules::Human::Tracking>();
            trackingData->_todo = localPlayer;

            _localPlayer.add<Core::Modules::Human::LocalPlayer>();
        });

        SetOnConnectionClosedCallback([this]() {
            _stateMachine->RequestNextState(States::StateIds::SessionDisconnection);
            
            if (_localPlayer.is_alive()) 
                _localPlayer.destruct();
        });

        // TEMP hook up human events
        const auto net = GetNetworkingEngine()->GetNetworkClient();

        net->RegisterMessage<Shared::Messages::Human::HumanSpawn>(Shared::Messages::ModMessages::MOD_HUMAN_SPAWN, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanSpawn *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            // setup tracking info

            // setup human on finished callback
            
            // call GetPlayerFactory()->SetupClient(e, guid)

            // add Human::Tracking component and store game pointers
        });
        net->RegisterMessage<Shared::Messages::Human::HumanDespawn>(Shared::Messages::ModMessages::MOD_HUMAN_DESPAWN, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanDespawn *msg) {
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            const auto trackingData = e.get<Core::Modules::Human::Tracking>();
            if (!trackingData) {
                return;
            }

            // request game actor despawn
        });
        net->RegisterMessage<Shared::Messages::Human::HumanUpdate>(Shared::Messages::ModMessages::MOD_HUMAN_UPDATE, [this](SLNet::RakNetGUID guid, Shared::Messages::Human::HumanUpdate *msg) {
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
