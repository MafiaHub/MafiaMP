#include "c_game_gui_2_module.h"

#include <utils/hooking/hooking.h>

#include "../../patterns.h"

namespace SDK {
    namespace mafia::ui {
        int C_GameGUI2Module::SendMessageMovie(char const* title, char const* msg, ue::C_Variant* varArgs, unsigned int unk2) {
            return hook::this_call<int>(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 48 8D 5D 58"), this, title, msg, varArgs, unk2);
        }

        C_GameGUI2Module *GetGameGui2Module() {
            return hook::this_call<C_GameGUI2Module *>(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 40 80 F6 01"));
        }
    } // namespace mafia::ui
}
