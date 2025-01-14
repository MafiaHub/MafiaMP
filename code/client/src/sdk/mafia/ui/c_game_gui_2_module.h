#pragma once

#include "menu/c_main_menu.h"
#include "menu/c_menu_base.h"

#include "hud/c_hud_controller.h"

#include "support/c_fader.h"

#include "../../ue/c_variant.h"
#include "../../ue/c_weak_ptr.h"
#include "../../ue/sys/sodb/c_database_interface.h"

#include <cstdint>

namespace SDK {
    namespace mafia::ui {
        class C_GameGUI2Module {
          public:
            char pad0[0x218];                               // 0000 - 0218
            hud::C_HudController *m_pHudController;         // 0218 - 0220
            char pad1[0x1E0];                               // 0220 - 0400
            support::C_Fader *m_pFader;                     // 0400 - 0408

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
                return *reinterpret_cast<mafia::ui::menu::C_MenuBase **>((uint64_t)this + 0x80);
            }

            mafia::ui::menu::C_MainMenu *GetMainMenu() {
                return *reinterpret_cast<mafia::ui::menu::C_MainMenu **>((uint64_t)this + 0x0C0);
            }

             ue::C_WeakPtr<ue::sys::sodb::C_DatabaseInterface> GetDatabase();

            static C_GameGUI2Module *GetInstance();
        };

        C_GameGUI2Module *GetGameGui2Module();
    } // namespace mafia::ui
} // namespace SDK
