#include "c_game_gui_2_module.h"

#include <utils/hooking/hooking.h>

#include "../../patterns.h"

namespace SDK {
    namespace mafia::ui {
        /* int C_GameGUI2Module::PlaySound(unsigned int index) {
            return 0;
            // return hook::this_call<int>(TODO, this, index);
        }*/

        C_GameGUI2Module *GetGameGui2Module() {
            return hook::this_call<C_GameGUI2Module *>(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 40 80 F6 01"));
        }
    } // namespace mafia::ui
}
