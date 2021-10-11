#include "module.h"

#include "../core/application.h"

#include <logging/logger.h>
#include <utils/hooking/hooking.h>

namespace MafiaMP::Game {
    Module *gModule = nullptr;

    Module::Module() {
        StaticRegister(this);
    }

    void Module::OnSysInit(SDK::I_TickedModuleCallEventContext &) {
        Framework::Logging::GetLogger("Module")->debug("OnSysInit called");

        // Create our core module application
        MafiaMP::Core::gApplication.reset(new MafiaMP::Core::Application);

        // Init our main application
        if (MafiaMP::Core::gApplication && !MafiaMP::Core::gApplication->IsInitialized()) {
            Framework::Graphics::RendererConfiguration rendererOptions;
            rendererOptions.backend = Framework::Graphics::RendererBackend::BACKEND_D3D_11;

            Framework::Integrations::Client::InstanceOptions opts;
            opts.discordAppId    = 763114144454672444;
            opts.useRenderer     = true;
            opts.usePresence     = true;
            opts.rendererOptions = rendererOptions;

            MafiaMP::Core::gApplication->Init(opts);
        }
    }

    void Module::OnSysShutdown(SDK::I_TickedModuleCallEventContext &) {
        Framework::Logging::GetLogger("Module")->debug("OnSysShutdown called");

        // Properly shutdown our main application
        if (MafiaMP::Core::gApplication && MafiaMP::Core::gApplication->IsInitialized()) {
            MafiaMP::Core::gApplication->Shutdown();
        }
        delete this;
    }

    void Module::OnAppActivate(SDK::I_TickedModuleCallEventContext &) {
        Framework::Logging::GetLogger("Module")->debug("OnAppActivate called");
    }

    void Module::OnAppDeactivate(SDK::I_TickedModuleCallEventContext &) {
        Framework::Logging::GetLogger("Module")->debug("OnAppDeactivate called");
    }

    void Module::OnGameInit(SDK::I_TickedModuleCallEventContext &) {
        Framework::Logging::GetLogger("Module")->debug("OnGameInit called");
    }

    void Module::OnGameShutdown(SDK::I_TickedModuleCallEventContext &) {
        Framework::Logging::GetLogger("Module")->debug("OnGameShutdown called");
    }

    void Module::OnGameTick(SDK::I_TickedModuleCallEventContext &) {
        // Tick our main application
        if (MafiaMP::Core::gApplication && MafiaMP::Core::gApplication->IsInitialized()) {
            MafiaMP::Core::gApplication->Update();
        }
    }

    void Module::OnGameRender(SDK::I_TickedModuleCallEventContext &) {
        // Tick our rendering thread
        if (MafiaMP::Core::gApplication && MafiaMP::Core::gApplication->IsInitialized()) {
            MafiaMP::Core::gApplication->Render();
        }
    }

    void Module::StaticRegister(Module *instance) {
        auto *mgr = SDK::GetTickedModuleManager();
        if (!mgr) {
            Framework::Logging::GetLogger("Module")->error("Failed to acquire C_TickedModuleManager instance");
            return;
        }

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_INIT, 9999, instance, (SDK::TickedModuleCallback)(&Module::OnSysInit), -1.0f, 0, 0, "[TM]Module::OnSysInit");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_GAME_PAUSED, instance, (SDK::TickedModuleCallback)(&Module::OnSysInit), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_DONE, 500, instance, (SDK::TickedModuleCallback)(&Module::OnSysShutdown), -1.0f, 0, 0, "[TM]Module::OnSysShutdown");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_DONE, instance, (SDK::TickedModuleCallback)(&Module::OnSysShutdown), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_GAME_INIT, 500, instance, (SDK::TickedModuleCallback)(&Module::OnGameInit), -1.0f, 0, 0, "[TM]Module::OnGameInit");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_GAME_INIT, instance, (SDK::TickedModuleCallback)(&Module::OnGameInit), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_RENDER, 99999, instance, (SDK::TickedModuleCallback)(&Module::OnGameRender), -1.0f, 0, 0, "[TM]Module::OnGameRender");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_RENDER, instance, (SDK::TickedModuleCallback)(&Module::OnGameRender), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_GAME_DONE, 1720, instance, (SDK::TickedModuleCallback)(&Module::OnGameShutdown), -1.0f, 0, 0, "[TM]Module::OnGameShutdown");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_GAME_DONE, instance, (SDK::TickedModuleCallback)(&Module::OnGameShutdown), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_TICK, 400, instance, (SDK::TickedModuleCallback)(&Module::OnGameTick), -1.0f, 0, 0, "[TM]Module::OnGameTick");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_TICK, instance, (SDK::TickedModuleCallback)(&Module::OnGameTick), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_APP_ACTIVATE, 400, instance, (SDK::TickedModuleCallback)(&Module::OnAppActivate), -1.0f, 0, 0, "[TM]Module::OnAppActivate");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_APP_ACTIVATE, instance, (SDK::TickedModuleCallback)(&Module::OnAppActivate), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_APP_DEACTIVATE, 400, instance, (SDK::TickedModuleCallback)(&Module::OnAppDeactivate), -1.0f, 0, 0, "[TM]Module::OnAppDeactivate");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_APP_DEACTIVATE, instance, (SDK::TickedModuleCallback)(&Module::OnAppDeactivate), true);

        Framework::Logging::GetLogger("Module")->info("Registration success");
    }

    void Module::StaticHandleShutdown(Module *) {
        Framework::Logging::GetLogger("Module")->info("Shutdown success");
        // TODO: find a way to properly shutdown the game
    }
} // namespace MafiaMP::Game
