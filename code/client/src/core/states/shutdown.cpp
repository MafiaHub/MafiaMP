#include "shutdown.h"
#include "states.h"

#include <utils/states/machine.h>

namespace MafiaMP::Core::States {
    ShutdownState::ShutdownState() {}

    ShutdownState::~ShutdownState() {}

    int32_t ShutdownState::GetId() const {
        return StateIds::Shutdown;
    }

    const char *ShutdownState::GetName() const {
        return "Shutdown";
    }

    bool ShutdownState::OnEnter(Framework::Utils::States::Machine *) {
        return true;
    }

    bool ShutdownState::OnExit(Framework::Utils::States::Machine *) {
        return true;
    }

    bool ShutdownState::OnUpdate(Framework::Utils::States::Machine *) {
        return true;
    }
} // namespace MafiaMP::Core::States
