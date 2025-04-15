#include "application_module.h"

#include <MinHook.h>
#include <logging/logger.h>
#include <utils/hooking/hooking.h>

#include "sdk/c_game.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_player_2.h"
#include "sdk/entities/c_vehicle.h"
#include "sdk/entities/human/c_human_weapon_controller.h"
#include "sdk/ue/c_application_win32.h"
#include "sdk/ue/sys/core/c_core.h"
#include "sdk/ue/sys/core/i_core.h"

#include "shared/version.h"

namespace MafiaMP::Core {
    ApplicationModule *gApplicationModule = nullptr;
    ApplicationModuleContext gApplicationModuleContext = {};

    ApplicationModule::ApplicationModule() {
        StaticRegister(this);
    }

    void ApplicationModule::OnSysInit(SDK::I_TickedModuleCallEventContext &) {
        // Create our core module application
        Core::gApplication.reset(new Core::Application);

        // Init our main application
        if (Core::gApplication && !Core::gApplication->IsInitialized()) {
            Framework::Graphics::RendererConfiguration rendererOptions = {};
            rendererOptions.backend  = Framework::Graphics::RendererBackend::BACKEND_D3D_11;
            rendererOptions.platform = Framework::Graphics::PlatformBackend::PLATFORM_WIN32;
            rendererOptions.d3d11.device                               = gApplicationModuleContext.renderDevice->GetDevice();
            rendererOptions.d3d11.deviceContext                        = gApplicationModuleContext.renderDevice->GetImmediateContext();
            rendererOptions.d3d11.swapChain                            = gApplicationModuleContext.swapChain;
            rendererOptions.windowHandle                               = gApplicationModuleContext.windowHandle;

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

    void ApplicationModule::OnRender(SDK::I_TickedModuleCallEventContext &) {
        const auto app = MafiaMP::Core::gApplication.get();

        if (!app || (app && !app->IsInitialized())) {
            return;
        }

        app->Render();
    }

    void ApplicationModule::OnSysShutdown(SDK::I_TickedModuleCallEventContext &) {
        if (Core::gApplication && Core::gApplication->IsInitialized()) {
            Core::gApplication->Shutdown();
        }
        delete this;
    }

    void ApplicationModule::OnTick(SDK::I_TickedModuleCallEventContext &) {
        if (!Core::gApplication || !Core::gApplication->IsInitialized()) {
            return;
        }

        Core::gApplication->Update();
    }

    void ApplicationModule::StaticRegister(ApplicationModule *instance) {
        auto *mgr = SDK::GetTickedModuleManager();
        if (!mgr) {
            Framework::Logging::GetLogger("ApplicationModule")->error("Failed to acquire C_TickedModuleManager instance");
            return;
        }

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_INIT, 9999, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnSysInit), -1.0f, 0, 0, "[MH]ApplicationModule::OnSysInit");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_INIT, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnSysInit), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_DONE, 500, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnSysShutdown), -1.0f, 0, 0, "[MH]ApplicationModule::OnSysShutdown");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_SYSTEM_DONE, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnSysShutdown), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_RENDER, 99999, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnRender), -1.0f, 0, 0, "[MH]ApplicationModule::OnRender");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_RENDER, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnRender), true);

        mgr->AddAction(SDK::E_TmEvent::E_TMEVENT_TICK, 400, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnTick), -1.0f, 0, 0, "[MH]ApplicationModule::OnTick");
        mgr->EnableAction(SDK::E_TmEvent::E_TMEVENT_TICK, instance, (SDK::TickedModuleCallback)(&ApplicationModule::OnTick), true);

        Framework::Logging::GetLogger("ApplicationModule")->info("Registration success");
    }

    void ApplicationModule::StaticHandleShutdown(ApplicationModule *) {
        Framework::Logging::GetLogger("ApplicationModule")->info("Shutdown success");
        // TODO: find a way to properly shutdown the game
    }
} // namespace MafiaMP::Game
