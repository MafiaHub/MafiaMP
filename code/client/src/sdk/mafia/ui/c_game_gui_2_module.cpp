#include "c_game_gui_2_module.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::ui {
        void C_GameGUI2Module::SendHUDSimpleBooleanMessage(char const *str, bool unk) {
            hook::this_call<void>(gPatterns.C_GameGUI2Module__SendHUDSimpleBooleanMessage, this, str, unk);
        }

        void C_GameGUI2Module::SendMessageMovie(char const *title, char const *msg, ue::C_Variant *varArgs, unsigned int unk2) {
            hook::this_call(gPatterns.C_GameGUI2Module__SendMessageMovie, this, title, msg, varArgs, unk2);
        }

        C_GameGUI2Module *C_GameGUI2Module::GetInstance() {
            return *reinterpret_cast<C_GameGUI2Module **>(gPatterns.C_GameGUI2Module__GetInstance);
        }

        C_GameGUI2Module *GetGameGui2Module() {
            return hook::call<C_GameGUI2Module *>(gPatterns.C_GameGUI2Module__GetGameGui2Module);
        }
    } // namespace mafia::ui
} // namespace SDK
