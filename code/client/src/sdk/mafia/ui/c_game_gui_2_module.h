#pragma once

#include <cstdint>

#include "menu/c_global_gui.h"
#include "menu/c_main_menu.h"
#include "menu/c_menu_base.h"
#include "hud/c_hud_controller.h"
#include "dialog/c_ui_game_dialog_manager.h"
#include "support/c_fader.h"

#include "sdk/c_ticked_module.h"
#include "sdk/mafia/ui/compexp/c_test_companion_experience_view.h"
#include "sdk/mafia/ui/controllers/c_mouse_cursor_controller.h"
#include "sdk/mafia/ui/dialog/c_game_dialog_manager.h"
#include "sdk/mafia/ui/hud/c_action_hints_manager.h"
#include "sdk/mafia/ui/hud/c_radar_controller.h"
#include "sdk/mafia/ui/hud/c_radar_drawer.h"
#include "sdk/mafia/ui/menu/c_crosshair_menu.h"
#include "sdk/mafia/ui/menu/c_customize_car_menu.h"
#include "sdk/mafia/ui/menu/c_entity_indicator_menu.h"
#include "sdk/mafia/ui/menu/c_garage_menu.h"
#include "sdk/mafia/ui/menu/c_hud.h"
#include "sdk/mafia/ui/menu/c_inventory_hud.h"
#include "sdk/mafia/ui/menu/c_loading_screen.h"
#include "sdk/mafia/ui/menu/c_map_screen.h"
#include "sdk/mafia/ui/menu/c_pause_menu.h"
#include "sdk/mafia/ui/menu/c_radar_menu.h"
#include "sdk/mafia/ui/menu/c_speedometer_menu.h"
#include "sdk/mafia/ui/menu/c_tutorial.h"
#include "sdk/mafia/ui/menu/c_vehicle_damage_timer.h"
#include "sdk/mafia/ui/menu/c_vehicle_delivery_menu.h"
#include "sdk/mafia/ui/menu/c_weapon_selection.h"
#include "sdk/mafia/ui/support/c_anim_loading_screen.h"
#include "sdk/mafia/ui/support/c_font_data_watcher.h"
#include "sdk/mafia/ui/support/c_gps_monitor.h"
#include "sdk/mafia/ui/support/c_ui_text__manager.h"
#include "sdk/ue/c_variant.h"
#include "sdk/ue/c_weak_ptr.h"
#include "sdk/ue/game/c_wiretap_zones_manager.h"
#include "sdk/ue/game/gui/c_gui_core.h"
#include "sdk/ue/sys/sodb/c_database_interface.h"

namespace SDK {
    namespace mafia::ui {
        class C_GameGUI2Module : public C_TickedModule {
          public:
            char pad0[0x60];                                        // 0008 - 0068
            ue::game::gui::C_GUICore *m_pGuiCore;                   // 0068 - 0070
            dialog::C_GameDialogManager *m_pGameDialogManager;      // 0070 - 0078
            dialog::C_UIGameDialogManager *m_pDialogManager;        // 0078 - 0080
            menu::C_MenuBase *m_pMenuBase;                          // 0080 - 0088
            menu::C_PauseMenu *m_pPauseMenu;                        // 0088 - 0090
            menu::C_HUD *m_pHud;                                    // 0090 - 0098
            menu::C_EntityIndicatorMenu *m_pEntityIndicatorMenu;    // 0098 - 00A0
            menu::C_RadarMenu *m_pRadarMenu;                        // 00A0 - 00A8
            menu::C_SpeedometerMenu *m_pSpeedometerMenu;            // 00A8 - 00B0
            menu::C_CrosshairMenu *m_pCrosshairMenu;                // 00B0 - 00B8
            menu::C_InventoryHUD *m_pInventoryHUD;                  // 00B8 - 00C0
            menu::C_MainMenu *m_pMainMenu;                          // 00C0 - 00C8
            menu::C_LoadingScreen *m_pLoadingScreen;                // 00C8 - 00D0
            menu::C_MapScreen *m_pMapScreen;                        // 00D0 - 00D8
            menu::C_GarageMenu *m_pGarageMenu;                      // 00D8 - 00E0
            char pad1[0x8];                                         // 00E0 - 00E8
            menu::C_CustomizeCarMenu *m_pCustomizeCarMenu;          // 00E8 - 00F0
            menu::C_WeaponSelection *m_pWeaponSelection;            // 00F0 - 00F8
            menu::C_Tutorial *m_pTutorial;                          // 00F8 - 0100
            menu::C_VehicleDeliveryMenu *m_pVehicleDeliveryMenu;    // 0100 - 0108
            menu::C_VehicleDamageTimer *m_pVehicleDamageTimer;      // 0108 - 0110
            void *m_pUnkMenu1;                                      // 0110 - 0118
            void *m_pUnkMenu2;                                      // 0118 - 0120
            char pad2[0xF8];                                        // 0120 - 0218
            hud::C_HudController *m_pHudController;                 // 0218 - 0220
            hud::C_ActionHintsManager *m_pActionHintsManager;       // 0220 - 0228
            hud::C_RadarController *m_pRadarController;             // 0228 - 0230
            char pad3[0x20];                                        // 0230 - 0250
            hud::C_RadarDrawer *m_pRadarDrawer;                     // 0250 - 0258
            char pad4[0x18];                                        // 0258 - 0270
            compexp::C_TestCompanionExperienceView *m_pCompanionExp;// 0270 - 0278
            char pad5[0x8];                                         // 0278 - 0280
            controllers::C_MouseCursorController *m_pMouseCursor;   // 0280 - 0288
            char pad6[0x118];                                       // 0288 - 03A0
            support::C_GPSMonitor *m_pGPSMonitor;                   // 03A0 - 03A8
            char pad7[0x58];                                        // 03A8 - 0400
            support::C_Fader *m_pFader;                             // 0400 - 0408
            support::C_AnimLoadingScreen *m_pAnimLoadingScreen;     // 0408 - 0410
            char pad8[0x8];                                         // 0410 - 0418
            support::C_UITextManager *m_pUITextManager;             // 0418 - 0420
            support::C_FontDataWatcher *m_pFontDataWatcher;         // 0420 - 0428
            char pad9[0x100];                                       // 0428 - 0528
            ue::game::C_WiretapZonesManager *m_pWiretapZonesManager;// 0528 - 0530

          public:
            void SendHUDSimpleBooleanMessage(char const *, bool);
            void SendMessageMovie(char const *, char const *, ue::C_Variant *, unsigned int);

            hud::C_HudController* GetHudController() {
                return m_pHudController;
            }

            support::C_Fader* GetFader() {
                return m_pFader;
            }

            mafia::ui::menu::C_MenuBase *GetMenuBase() {
                return m_pMenuBase;
            }

            mafia::ui::menu::C_MainMenu *GetMainMenu() {
                return m_pMainMenu;
            }

             ue::C_WeakPtr<ue::sys::sodb::C_DatabaseInterface> GetDatabase();

            static C_GameGUI2Module *GetInstance();
        };

        C_GameGUI2Module *GetGameGui2Module();
    } // namespace mafia::ui
} // namespace SDK
