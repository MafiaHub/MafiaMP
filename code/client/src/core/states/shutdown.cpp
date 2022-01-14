#include "shutdown.h"
#include "states.h"

namespace MafiaMP::Core::States {
    ShutdownState::ShutdownState() {}

    ShutdownState::~ShutdownState() {}

    int32_t ShutdownState::GetId() const {
        return StateIds::Shutdown;
    }

    const char *ShutdownState::GetName() const {
        return "Shutdown";
    }

    bool ShutdownState::OnEnter() {
        return true;
    }

    bool ShutdownState::OnExit() {
        return true;
    }

    bool ShutdownState::OnUpdate() {
        return true;
    }
} // namespace MafiaMP::Core::States
