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

#include "../shared/messages/human/human_spawn.h"
#include "../shared/messages/human/human_despawn.h"
#include "../shared/messages/human/human_update.h"
#include "../shared/messages/human/human_self_update.h"

#include "../shared/modules/human_sync.hpp"

#include "../game/helpers/controls.h"
#include "external/imgui/widgets/corner_text.h"

#include <utils/version.h>
#include "../shared/version.h"

#include "modules/human.hpp"

#include "../sdk/ue/sys/utils/c_hash_name.h"
#include "../sdk/entities/c_entity_list.h"
#include "../sdk/entities/c_door.h"

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    bool Application::PostInit() {
        // Create the state machine and initialize
        _stateMachine = std::make_shared<Framework::Utils::States::Machine>();
        _stateMachine->RegisterState<States::InitializeState>();
        _stateMachine->RegisterState<States::InMenuState>();
        _stateMachine->RegisterState<States::SessionConnectionState>();
        _stateMachine->RegisterState<States::SessionConnectedState>();
        _stateMachine->RegisterState<States::SessionDisconnectionState>();
        _stateMachine->RegisterState<States::SessionOfflineDebugState>();
        _stateMachine->RegisterState<States::ShutdownState>();

        // Create the entity factory
        _entityFactory = std::make_shared<Game::Streaming::EntityFactory>();

        // Register other things
        InitNetworkingMessages();

        // This must always be the last call
        _stateMachine->RequestNextState(States::StateIds::Initialize);

        _console = std::make_unique<UI::Console>(_stateMachine);

        // Register client modules
        GetWorldEngine()->GetWorld()->import<Modules::Human>();
        GetWorldEngine()->GetWorld()->import<Shared::Modules::HumanSync>();

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

        // Tick discord instance - Temporary
        const auto discordApi = Core::gApplication->GetPresence();
        if (discordApi && discordApi->IsInitialized()) {
            discordApi->SetPresence("Freeroam", "Screwing around", discord::ActivityType::Playing);
        }
#if 1
        if (GetAsyncKeyState(VK_F8) & 0x1) {
            _console->Toggle();
        }

        _console->Update();
#endif
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
        });
#endif
    }

    void Application::InitNetworkingMessages() {
        SetOnConnectionFinalizedCallback([this](flecs::entity newPlayer, float tickInterval) {
            _tickInterval = tickInterval;
            _stateMachine->RequestNextState(States::StateIds::SessionConnected);
            auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();

            _localPlayer = newPlayer;

            auto trackingData   = _localPlayer.get_mut<Core::Modules::Human::Tracking>();
            trackingData->human = localPlayer;
            trackingData->info  = nullptr;

            _localPlayer.add<Shared::Modules::HumanSync::UpdateData>();
            _localPlayer.add<Core::Modules::Human::LocalPlayer>();

            const auto es                  = _localPlayer.get_mut<Framework::World::Modules::Base::Streamable>();
            es->modEvents.updateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                const auto updateData = e.get<Shared::Modules::HumanSync::UpdateData>();

                Shared::Messages::Human::HumanUpdate humanUpdate;
                humanUpdate.FromParameters(GetWorldEngine()->GetServerID(e));
                humanUpdate.SetCharStateHandlerType(updateData->_charStateHandlerType);
                humanUpdate.SetHealthPercent(updateData->_healthPercent);
                humanUpdate.SetMoveMode(updateData->_moveMode);
                humanUpdate.SetSprinting(updateData->_isSprinting);
                humanUpdate.SetSprintSpeed(updateData->_sprintSpeed);
                humanUpdate.SetStalking(updateData->_isStalking);
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

            // Setup tracking info
            Core::Modules::Human::CreateHuman(e, msg->GetSpawnProfile());

            // Setup other components
            e.add<Shared::Modules::HumanSync::UpdateData>();

            // set up client updates (NPC streaming)
            // TODO disabled for now, we don't really need to stream NPCs atm
#if 0
            const auto es = e.get_mut<Framework::World::Modules::Base::Streamable>();
            es->modEvents.clientUpdateProc = [&](Framework::Networking::NetworkPeer *peer, uint64_t guid, flecs::entity e) {
                Shared::Messages::Human::HumanClientUpdate humanUpdate;
                humanUpdate.FromParameters(e.id());
                // set up sync data
                peer->Send(humanUpdate, guid);
                return true;
            };
#endif
            SDK::ue::sys::utils::C_HashName doorHash = SDK::ue::sys::utils::C_HashName::ComputeHash("bc_commercial_door_a_bv01_00");
            auto door                                = SDK::GetEntityList()->GetEntityByName(doorHash, false);
            Framework::Logging::GetInstance()->Get("aaa")->info("ccc");
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

            auto updateData = e.get_mut<Shared::Modules::HumanSync::UpdateData>();
            updateData->_charStateHandlerType = msg->GetCharStateHandlerType();
            updateData->_healthPercent        = msg->GetHealthPercent();
            updateData->_isSprinting          = msg->IsSprinting();
            updateData->_isStalking           = msg->IsStalking();
            updateData->_moveMode             = msg->GetMoveMode();
            updateData->_sprintSpeed          = msg->GetSprintSpeed();

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
