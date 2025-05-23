#include "c_game_gui_2_module.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::ui {
        void C_GameGUI2Module::SendHUDSimpleBooleanMessage(char const *str, bool unk) {
            hook::this_call<void>(gPatterns.C_GameGUI2Module__SendHUDSimpleBooleanMessage, this, str, unk);
        }

        void C_GameGUI2Module::SendMessageMovie(char const *pZone, char const *pType, ue::C_Variant *varArgs, unsigned int unk2) {
            hook::this_call(gPatterns.C_GameGUI2Module__SendMessageMovie, this, pZone, pType, varArgs, unk2);
        }

        ue::C_WeakPtr<ue::sys::sodb::C_DatabaseInterface> C_GameGUI2Module::GetDatabase() {
            ue::C_WeakPtr<ue::sys::sodb::C_DatabaseInterface> database;
            return hook::this_call<ue::C_WeakPtr<ue::sys::sodb::C_DatabaseInterface> &>(gPatterns.C_GameGUI2Module__GetDatabase, this, database);
        }

        C_GameGUI2Module *C_GameGUI2Module::GetInstance() {
            return *reinterpret_cast<C_GameGUI2Module **>(gPatterns.C_GameGUI2Module__Instance);
        }

        C_GameGUI2Module *GetGameGui2Module() {
            return hook::call<C_GameGUI2Module *>(gPatterns.C_GameGUI2Module__GetGameGui2Module);
        }
    } // namespace mafia::ui
} // namespace SDK
