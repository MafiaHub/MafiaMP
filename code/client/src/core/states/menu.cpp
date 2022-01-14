#include "menu.h"
#include "states.h"

namespace MafiaMP::Core::States {
    InMenuState::InMenuState() {}

    InMenuState::~InMenuState() {}

    int32_t InMenuState::GetId() const {
        return StateIds::Menu;
    }

    const char *InMenuState::GetName() const {
        return "InMenu";
    }

    bool InMenuState::OnEnter() {
        return true;
    }

    bool InMenuState::OnExit() {
        return true;
    }

    bool InMenuState::OnUpdate() {
        return true;
    }
} // namespace MafiaMP::Core::States
