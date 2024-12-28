#include "module.h"

#include "sdk/c_game.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/entities/human/c_human_weapon_controller.h"

#include <logging/logger.h>
#include <utils/hooking/hooking.h>

#include <MinHook.h>

#include "shared/version.h"

namespace MafiaMP::Game {
    Globals gGlobals;

    ModModule::ModModule() {
        StaticRegister(this);
    }

    void ModModule::OnSysInit(SDK::I_TickedModuleCallEventContext &) {
        // Create our core module application
        Core::gApplication.reset(new Core::Application);

        // Init our main application
        if (Core::gApplication && !Core::gApplication->IsInitialized()) {
            Framework::Graphics::RendererConfiguration rendererOptions = {};

            rendererOptions.backend  = Framework::Graphics::RendererBackend::BACKEND_D3D_11;
            rendererOptions.platform = Framework::Graphics::PlatformBackend::PLATFORM_WIN32;

            // fill out renderer info
            rendererOptions.d3d11.device        = gGlobals.renderDevice->GetDevice();
            rendererOptions.d3d11.deviceContext = gGlobals.renderDevice->GetImmediateContext();
            rendererOptions.windowHandle        = gGlobals.window;

            Framework::Integrations::Client::InstanceOptions opts;
            opts.discordAppId    = 763114144454672444;
            opts.useRenderer     = true;
            opts.usePresence     = true;
            opts.useImGUI        = true;
            opts.rendererOptions = rendererOptions;

            opts.gameName    = "Mafia: Definitive Edition";
            opts.gameVersion = "3168979183"; // TODO: get this crc32 checksum from the game executable
            opts.modVersion  = MafiaMP::Version::rel;

            // opts.modVersion = MafiaMP::Version::rel;

            Core::gApplication->Init(opts);
        }
    }

    void ModModule::OnGameRender(SDK::I_TickedModuleCallEventContext &) {
        const auto app = MafiaMP::Core::gApplication.get();

        if (!app || (app && !app->IsInitialized()))
            return;

        // Tick our rendering thread
        app->Render();
    }

    void ModModule::OnSysShutdown(SDK::I_TickedModuleCallEventContext &) {
        // Properly shutdown our main application
        if (Core::gApplication && Core::gApplication->IsInitialized()) {
            Core::gApplication->Shutdown();
        }
        delete this;
    }

    void ModModule::OnGameTick(SDK::I_TickedModuleCallEventContext &) {
        if (!Core::gApplication || !Core::gApplication->IsInitialized()) {
            return;
        }

        Core::gApplication->Update();
    }

    void ModModule::StaticRegister(ModModule *instance) {
        auto *mgr = SDK::GetTickedModuleManager();
        if (!mgr) {
            Framework::Logging::GetLogger("ModModule")->error("Failed to acquire C_TickedModuleManager instance");
            return;
        }

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_INIT, 9999, instance, (SDK::TickedModuleCallback)(&ModModule::OnSysInit), -1.0f, 0, 0, "[TM]ModModule::OnSysInit");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_INIT, instance, (SDK::TickedModuleCallback)(&ModModule::OnSysInit), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_DONE, 500, instance, (SDK::TickedModuleCallback)(&ModModule::OnSysShutdown), -1.0f, 0, 0, "[TM]ModModule::OnSysShutdown");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_DONE, instance, (SDK::TickedModuleCallback)(&ModModule::OnSysShutdown), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_RENDER, 99999, instance, (SDK::TickedModuleCallback)(&ModModule::OnGameRender), -1.0f, 0, 0, "[TM]ModModule::OnGameRender");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_RENDER, instance, (SDK::TickedModuleCallback)(&ModModule::OnGameRender), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_TICK, 400, instance, (SDK::TickedModuleCallback)(&ModModule::OnGameTick), -1.0f, 0, 0, "[TM]ModModule::OnGameTick");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_TICK, instance, (SDK::TickedModuleCallback)(&ModModule::OnGameTick), true);

        Framework::Logging::GetLogger("ModModule")->info("Registration success");
    }

    void ModModule::StaticHandleShutdown(ModModule *) {
        Framework::Logging::GetLogger("ModModule")->info("Shutdown success");
        // TODO: find a way to properly shutdown the game
    }
} // namespace MafiaMP::Game
