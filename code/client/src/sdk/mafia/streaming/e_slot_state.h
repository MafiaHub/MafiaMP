#pragma once

namespace SDK {
    namespace mafia::streaming {
        enum E_SlotState {
            E_SLOT_STATE_UNKNOWN,
            E_SLOT_STATE_NOEXIST,
            E_SLOT_STATE_EMPTY,
            E_SLOT_STATE_WAITING_FOR_LOAD,
            E_SLOT_STATE_LOADING,
            E_SLOT_STATE_ACTIVATE = 6,
            E_SLOT_STATE_FULL,
            E_SLOT_STATE_DEACTIVATE,
            E_SLOT_STATE_DELETING,
            E_SLOT_STATE_FAIL
        };
    }
} // namespace SDK
