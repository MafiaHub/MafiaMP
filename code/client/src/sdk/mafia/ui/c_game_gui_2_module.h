#pragma once

#include <cstdint>

#include "menu/c_main_menu.h"
#include "menu/c_menu_base.h"
#include "hud/c_hud_controller.h"
#include "dialog/c_ui_game_dialog_manager.h"
#include "support/c_fader.h"

#include "sdk/c_ticked_module.h"
#include "sdk/ue/c_variant.h"
#include "sdk/ue/c_weak_ptr.h"
#include "sdk/ue/sys/sodb/c_database_interface.h"

namespace SDK {
    namespace mafia::ui {
        class C_GameGUI2Module : public C_TickedModule {
          public:
            char pad0[0x70];                                        // 0008 - 0078
            dialog::C_UIGameDialogManager *m_pDialogManager;        // 0078 - 0080
            ui::menu::C_MenuBase *m_pMenuBase;                      // 0080 - 0088
            char pad1[0x38];                                        // 0088 - 00C0
            ui::menu::C_MainMenu *m_pMainMenu;                      // 00C0 - 00C8
            char pad2[0x150];                                       // 00C8 - 0218
            hud::C_HudController *m_pHudController;                 // 0218 - 0220
            char pad3[0x1E0];                                       // 0220 - 0400
            support::C_Fader *m_pFader;                             // 0400 - 0408

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
