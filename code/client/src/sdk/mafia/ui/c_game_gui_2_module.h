#pragma once

#include "menu/c_main_menu.h"
#include "menu/c_menu_base.h"

#include "hud/c_hud_controller.h"

#include "../../ue/c_variant.h"

#include <cstdint>

namespace SDK {
    namespace mafia::ui {
        class C_GameGUI2Module {
          public:
            char pad0[0x218];                               // 0000 - 0218
            hud::C_HudController *m_pHudController;         // 0218 - 0220

          public:
            void SendHUDSimpleBooleanMessage(char const *, bool);
            void SendMessageMovie(char const *, char const *, ue::C_Variant *, unsigned int);

            hud::C_HudController* GetHudController() {
                return m_pHudController;
            }

            mafia::ui::menu::C_MenuBase *GetMenuBase() {
                return *reinterpret_cast<mafia::ui::menu::C_MenuBase **>((uint64_t)this + 0x80);
            }

            mafia::ui::menu::C_MainMenu *GetMainMenu() {
                return *reinterpret_cast<mafia::ui::menu::C_MainMenu **>((uint64_t)this + 0x0C0);
            }

            static C_GameGUI2Module *GetInstance();
        };

        C_GameGUI2Module *GetGameGui2Module();
    } // namespace mafia::ui
} // namespace SDK
