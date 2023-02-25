#include "c_game_gui_2_module.h"

#include <utils/hooking/hooking.h>

#include "../../patterns.h"

namespace SDK {
    namespace mafia::ui {
        void C_GameGUI2Module::SendHUDSimpleBooleanMessage(char const *str, bool unk) {
            hook::this_call<void>(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 49 8B 97 ? ? ? ? 4C 8D 05 ? ? ? ?"), this, str, unk);
        }

        void C_GameGUI2Module::SendMessageMovie(char const* title, char const* msg, ue::C_Variant* varArgs, unsigned int unk2) {
            hook::this_call(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 48 8D 5D 58"), this, title, msg, varArgs, unk2);
        }

        C_GameGUI2Module *C_GameGUI2Module::GetInstance() {
            return *reinterpret_cast<C_GameGUI2Module **>(gPatterns.C_GameGUI2Module_Instance);
        }

        C_GameGUI2Module *GetGameGui2Module() {
            return hook::call<C_GameGUI2Module *>(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 40 80 F6 01"));
        }
    } // namespace mafia::ui
}
