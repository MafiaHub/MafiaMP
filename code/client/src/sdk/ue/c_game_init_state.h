#pragma once

#include <sdk/ue/c_game_fsm_state.h>

namespace SDK {
    namespace ue {
        class C_GameInitState : public C_FsmState {
          public:
            C_FsmState *_pNextState; // 0078 - 0080

          public:
            virtual ~C_GameInitState() = default;

            C_FsmState *ProcessEvent(const C_FsmEvent *event) override {
                if (event->_eventType != 0) {
                    return nullptr;
                }
                return _pNextState;
            }

            C_FsmState *GetNextState() const {
                return _pNextState;
            }

            void SetNextState(C_FsmState *state) {
                _pNextState = state;
            }
        };

        static_assert(sizeof(C_GameInitState) == 0x80, "C_GameInitState size mismatch");

    } // namespace ue
} // namespace SDK
