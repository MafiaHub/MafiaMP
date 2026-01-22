#pragma once

#include <sdk/ue/c_game_fsm_state.h>

namespace SDK {
    namespace ue {
        class C_GameFSM {
          public:
            C_FsmState *_pCurrentState;  // 0008 - 0010
            C_FsmState *_pPreviousState; // 0010 - 0018

          public:
            virtual ~C_GameFSM() = default;

            C_FsmState *GetCurrentState() const {
                return _pCurrentState;
            }

            C_FsmState *GetPreviousState() const {
                return _pPreviousState;
            }

            void SetCurrentState(C_FsmState *state) {
                _pPreviousState = _pCurrentState;
                _pCurrentState  = state;
            }
        };

    } // namespace ue
} // namespace SDK
