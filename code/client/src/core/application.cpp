#include "application.h"

#include <logging/logger.h>

#include "states/initialize.h"
#include "states/menu.h"
#include "states/session_connection.h"
#include "states/session_connected.h"
#include "states/session_disconnection.h"
#include "states/shutdown.h"
#include "states/states.h"

#include "../game/helpers/controls.h"

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
    }

    void Application::InitNetworkingMessages() {
        SetOnConnectionFinalizedCallback([this](flecs::entity_t entityID) {
            _stateMachine->RequestNextState(States::StateIds::SessionConnected);

            auto guid = GetNetworkingEngine()->GetNetworkClient()->GetPeer()->GetMyGUID();
            auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();

            _localPlayer = GetPlayerFactory()->CreateClient(guid.g, localPlayer, entityID);
        });

        SetOnConnectionClosedCallback([this]() {
            _stateMachine->RequestNextState(States::StateIds::SessionDisconnection);
            _localPlayer.destruct();
        });
    }
} // namespace MafiaMP::Core
