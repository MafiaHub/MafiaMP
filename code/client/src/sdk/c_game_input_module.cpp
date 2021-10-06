#include "c_game_input_module.h"

#include "patterns.h"

namespace SDK {
    C_GameInputModule *GetGameInputModule() {
        return hook::call<C_GameInputModule *>(gPatterns.C_GameInputModule__GetGameInputModule);
    }

    void C_GameInputModule::PauseInput(bool paused) {
        hook::this_call<void>(gPatterns.C_GameInputModule__PauseInput, this, paused);
    }
} // namespace SDK