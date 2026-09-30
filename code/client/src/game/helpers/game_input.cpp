#include "game_input.h"

#include "core/application.h"

namespace MafiaMP::Game {
    bool GameInput::IsInputLocked() const {
        return Core::gApplication->AreControlsLocked();
    }

    void GameInput::SetInputLocked(bool locked) {
        Core::gApplication->LockControls(locked);
    }

} // namespace MafiaMP::Game
