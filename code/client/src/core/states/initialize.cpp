#include "initialize.h"

#include "states.h"

namespace MafiaMP::Core::States {
    InitializeState::InitializeState() {

    }

    InitializeState::~InitializeState() {

    }

    int32_t InitializeState::GetId() const {
        return StateIds::Initialize;
    }

    const char* InitializeState::GetName() const {
        return "Initialize";
    }

    bool InitializeState::OnEnter() {
        return true;
    }

    bool InitializeState::OnExit() {
        return true;
    }

    bool InitializeState::OnUpdate() {
        return false;
    }
}
