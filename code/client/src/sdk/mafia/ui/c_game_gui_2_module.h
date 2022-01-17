#pragma once

#include "menu/c_main_menu.h"
#include "menu/c_menu_base.h"

#include "../../ue/c_variant.h"

#include <cstdint>

namespace SDK {
    namespace mafia::ui {
        class C_GameGUI2Module {
          public:
            int SendMessageMovie(char const *, char const *, ue::C_Variant *, unsigned int);

            mafia::ui::menu::C_MenuBase *GetMenuBase() {
                return *reinterpret_cast<mafia::ui::menu::C_MenuBase **>((uint64_t)this + 0x80);
            }

            mafia::ui::menu::C_MainMenu *GetMainMenu() {
                return *reinterpret_cast<mafia::ui::menu::C_MainMenu **>((uint64_t)this + 0x0C0);
            }
        };

        C_GameGUI2Module *GetGameGui2Module();
    } // namespace mafia::ui
} // namespace SDK
