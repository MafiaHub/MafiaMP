#include "application.h"

#include <logging/logger.h>

#include "states/initialize.h"
#include "states/main_menu.h"
#include "states/session_connected.h"
#include "states/session_connection.h"
#include "states/session_disconnection.h"
#include "states/session_offline_debug.h"
#include "states/shutdown.h"
#include "states/states.h"

#include "game/helpers/controls.h"
#include "game/streaming/entity_factory.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/ue/gfx/environmenteffects/c_gfx_environment_effects.h"

#include "external/imgui/widgets/corner_text.h"

#include "shared/version.h"
#include <cppfs/fs.h>
#include <utils/version.h>

#include "shared/modules/human_sync.hpp"
#include "shared/modules/mod.hpp"
#include "shared/modules/vehicle_sync.hpp"

#include "shared/rpc/chat_message.h"
#include "shared/rpc/environment.h"
#include "shared/rpc/spawn_car.h"

#include "world/game_rpc/set_transform.h"

#include "modules/human.h"
#include "modules/vehicle.h"

namespace MafiaMP::Core {
    std::unique_ptr<Application> gApplication = nullptr;

    bool Application::PostInit() {
        // Create the state machine and initialize
        _stateMachine = std::make_shared<Framework::Utils::States::Machine>();
        _stateMachine->RegisterState<States::InitializeState>();
        _stateMachine->RegisterState<States::MainMenuState>();
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

        _commandProcessor = std::make_shared<Framework::Utils::CommandProcessor>();
        _input            = std::make_shared<MafiaMP::Game::GameInput>();
        _console          = std::make_shared<UI::MafiaConsole>(_commandProcessor, _input);
        _chat             = std::make_shared<UI::Chat>();
        _web              = std::make_shared<UI::Web>();

        if (_web) {
            _web->Init();
        }

        _chat->SetOnMessageSentCallback([this](const std::string &msg) {
            const auto net = gApplication->GetNetworkingEngine()->GetNetworkClient();

            MafiaMP::Shared::RPC::ChatMessage chatMessage {};
            chatMessage.FromParameters(msg);
            net->SendRPC(chatMessage, SLNet::UNASSIGNED_RAKNET_GUID);
        });

        // setup debug routines
        _devFeatures.Init();

        // Register client modules (sync)
        GetWorldEngine()->GetWorld()->import <Shared::Modules::Mod>();
        GetWorldEngine()->GetWorld()->import <Shared::Modules::HumanSync>();
        GetWorldEngine()->GetWorld()->import <Shared::Modules::VehicleSync>();

        // Register client modules
        GetWorldEngine()->GetWorld()->import <Modules::Human>();
        GetWorldEngine()->GetWorld()->import <Modules::Vehicle>();

        GetWorldEngine()->SetOnEntityDestroyCallback([](flecs::entity e) {
            const auto ekind = e.get<Shared::Modules::Mod::EntityKind>();
            switch (ekind->kind) {
            case Shared::Modules::Mod::MOD_PLAYER: Core::Modules::Human::Remove(e); break;
            case Shared::Modules::Mod::MOD_VEHICLE: Core::Modules::Vehicle::Remove(e); break;
            }

            return true;
        });

        // Setup Lua VM wrapper
        _luaVM = std::make_shared<LuaVM>();

        return true;
    }

    bool Application::PreShutdown() {
        if (_entityFactory) {
            _entityFactory->ReturnAll();
        }

        _devFeatures.Shutdown();

        return true;
    }

    void Application::PostUpdate() {
        if (_stateMachine) {
            _stateMachine->Update();
        }

        if (_entityFactory) {
            _entityFactory->Update();
        }

        static bool isStyleInitialized = false;
        if (!isStyleInitialized) {
            PimpMyImGUI();
            isStyleInitialized = true;
        }

        // Tick discord instance - Temporary
        const auto discordApi = Core::gApplication->GetPresence();
        if (discordApi && discordApi->IsInitialized()) {
            discordApi->SetPresence("Freeroam", "Screwing around", discord::ActivityType::Playing);
        }

#if 1
        Core::gApplication->GetImGUI()->PushWidget([&]() {
            using namespace Framework::External::ImGUI::Widgets;
            const auto networkClient = Core::gApplication->GetNetworkingEngine()->GetNetworkClient();
            const auto connState     = networkClient->GetConnectionState();
            const auto ping          = networkClient->GetPing();

            _console->Update();
            _devFeatures.Update();

            if (_input->IsKeyPressed(FW_KEY_F8)) {
                _console->Toggle();
            }

            const char *connStateNames[] = {"Connecting", "Online", "Offline"};

            // versioning
            DrawCornerText(CORNER_RIGHT_BOTTOM, "Mafia: Multiplayer");
            DrawCornerText(CORNER_RIGHT_BOTTOM, fmt::format("Framework version: {} ({})", Framework::Utils::Version::rel, Framework::Utils::Version::git));
            DrawCornerText(CORNER_RIGHT_BOTTOM, fmt::format("MafiaMP version: {} ({})", MafiaMP::Version::rel, MafiaMP::Version::git));

            // connection details
            DrawCornerText(CORNER_LEFT_BOTTOM, fmt::format("Connection: {}", connStateNames[connState]));
            DrawCornerText(CORNER_LEFT_BOTTOM, fmt::format("Ping: {}", ping));
        });
#endif

        if (_input) {
            _input->Update();
        }

        if (_web) {
            _web->Update();
        }

        if (GetAsyncKeyState(VK_F6) & 0x1) {
            // _web->CreateView("test", 500, 500, "https://youtube.fr");
        }
    }

    void Application::PostRender() {
        if (_web) {
            _web->Render();
        }
    }

    void Application::InitNetworkingMessages() {
        SetOnConnectionFinalizedCallback([this](flecs::entity newPlayer, float tickInterval) {
            _tickInterval = tickInterval;
            _stateMachine->RequestNextState(States::StateIds::SessionConnected);
            _localPlayer = newPlayer;
            Core::Modules::Human::SetupLocalPlayer(this, newPlayer);

            Framework::Logging::GetLogger(FRAMEWORK_INNER_NETWORKING)->info("Connection established!");
        });

        SetOnConnectionClosedCallback([this]() {
            _stateMachine->RequestNextState(States::StateIds::SessionDisconnection);
            Framework::Logging::GetLogger(FRAMEWORK_INNER_NETWORKING)->info("Connection lost!");
        });

        InitRPCs();

        Modules::Human::SetupMessages(this);
        Modules::Vehicle::SetupMessages(this);

        Framework::Logging::GetLogger(FRAMEWORK_INNER_NETWORKING)->info("Networking messages registered!");
    }

    void Application::PimpMyImGUI() {
        ImVec4 *colors                         = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.72f, 0.00f, 0.00f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.00f, 0.00f, 0.40f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.83f, 0.00f, 0.00f, 0.67f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.41f, 0.03f, 0.03f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.22f, 0.00f, 0.01f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.55f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.78f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
        colors[ImGuiCol_Button]                = ImVec4(0.70f, 0.00f, 0.00f, 0.40f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.61f, 0.01f, 0.09f, 1.00f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.78f, 0.02f, 0.02f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.75f, 0.00f, 0.00f, 0.31f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.78f, 0.00f, 0.00f, 0.80f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.86f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_Separator]             = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
        colors[ImGuiCol_Tab]                   = ImVec4(0.42f, 0.00f, 0.00f, 0.86f);
        colors[ImGuiCol_TabHovered]            = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
        colors[ImGuiCol_TabActive]             = ImVec4(0.67f, 0.01f, 0.01f, 1.00f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(0.23f, 0.00f, 0.00f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.36f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.55f, 0.02f, 0.02f, 0.35f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.62f, 0.01f, 0.01f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        ImGui::GetStyle().FrameRounding            = 0.5f;
        ImGui::GetStyle().WindowRounding           = 0.5f;
        ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
        ImGui::GetStyle().WindowTitleAlign         = {0.5f, 0.5f};
    }

    void Application::LockControls(bool lock) {
        if (lock) {
            _controlsLocked++;
        }
        else {
            _controlsLocked = std::max(--_controlsLocked, 0);
        }

        if (_controlsLocked) {
            // Lock game controls
            Game::Helpers::Controls::Lock(true);

            // Enable cursor
            GetImGUI()->ShowCursor(true);
        }
        else {
            // Unlock game controls
            Game::Helpers::Controls::Lock(false);

            // Disable cursor
            GetImGUI()->ShowCursor(false);
        }
    }

    uint64_t Application::GetLocalPlayerID() {
        if (!_localPlayer)
            return 0;

        const auto sid = _localPlayer.get<Framework::World::Modules::Base::ServerID>();
        return sid->id;
    }

    uint64_t Application::GetLocalPlayerOwnerID() {
        if (!_localPlayer)
            return 0;

        const auto str = _localPlayer.get<Framework::World::Modules::Base::Streamable>();
        return str->owner;
    }

    void Application::InitRPCs() {
        const auto net = GetNetworkingEngine()->GetNetworkClient();

        net->RegisterRPC<Shared::RPC::ChatMessage>([this](SLNet::RakNetGUID guid, Shared::RPC::ChatMessage *chatMessage) {
            if (!chatMessage->Valid())
                return;
            _chat->AddMessage(chatMessage->GetText());

            Framework::Logging::GetLogger("chat")->trace(chatMessage->GetText());
        });
        net->RegisterRPC<Shared::RPC::SetEnvironment>([this](SLNet::RakNetGUID guid, Shared::RPC::SetEnvironment *environmentMsg) {
            if (!environmentMsg->Valid()) {
                return;
            }

            const auto gfx = SDK::ue::gfx::environmenteffects::C_GfxEnvironmentEffects::GetInstance();

            if (environmentMsg->GetWeatherSet().HasValue())
                gfx->GetWeatherManager()->SetWeatherSet(environmentMsg->GetWeatherSet().Value().C_String(), 1.0f);
            if (environmentMsg->GetDayTimeHours().HasValue())
                gfx->GetWeatherManager()->SetDayTimeHours(environmentMsg->GetDayTimeHours().Value());
        });
        net->RegisterGameRPC<Framework::World::RPC::SetTransform>([this](SLNet::RakNetGUID guid, Framework::World::RPC::SetTransform *msg) {
            if (!msg->Valid()) {
                return;
            }
            const auto e = GetWorldEngine()->GetEntityByServerID(msg->GetServerID());
            if (!e.is_alive()) {
                return;
            }

            auto tr = e.get_mut<Framework::World::Modules::Base::Transform>();
            *tr     = msg->GetTransform();

            const auto ekind = e.get<Shared::Modules::Mod::EntityKind>();
            if (!ekind)
                return;
            switch (ekind->kind) {
            case Shared::Modules::Mod::MOD_PLAYER: Core::Modules::Human::UpdateTransform(e); break;
            case Shared::Modules::Mod::MOD_VEHICLE: Core::Modules::Vehicle::UpdateTransform(e); break;
            }
        });
    }

    std::string gProjectPath;
} // namespace MafiaMP::Core
