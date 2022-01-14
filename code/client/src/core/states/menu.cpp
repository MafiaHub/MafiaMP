#include "menu.h"
#include "states.h"

#include <utils/states/machine.h>

namespace MafiaMP::Core::States {
    InMenuState::InMenuState() {}

    InMenuState::~InMenuState() {}

    int32_t InMenuState::GetId() const {
        return StateIds::Menu;
    }

    const char *InMenuState::GetName() const {
        return "InMenu";
    }

    bool InMenuState::OnEnter(Framework::Utils::States::Machine *) {
        return true;
    }

    bool InMenuState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool InMenuState::OnUpdate(Framework::Utils::States::Machine *) {
        return true;
    }
} // namespace MafiaMP::Core::States
