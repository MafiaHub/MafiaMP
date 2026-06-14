#pragma once

#include <sdk/ue/c_fsm_event.h>

#include <stdint.h>

namespace SDK {
    namespace ue {
        class C_FsmState;

        class C_GameFSMState {
          public:
            float _fAvailableBudget;    // 0008 - 000C (C_TimeBudgetInfo)
            uint32_t _unk0C;            // 000C - 0010
            uint8_t _unk10;             // 0010 - 0011
            uint8_t _pad11[3];          // 0011 - 0014
            uint32_t _unk14;            // 0014 - 0018
            uint64_t _unk18;            // 0018 - 0020
            uint64_t _tickDuration;     // 0020 - 0028
            uint64_t _unk28;            // 0028 - 0030
            uint32_t _unk30;            // 0030 - 0034
            uint32_t _randomValue;      // 0034 - 0038 (initialized with rand())
            uint8_t _stateData[64];     // 0038 - 0078

          public:
            virtual ~C_GameFSMState() = default;
            virtual void *GetClassTypeInfo() = 0;
            virtual void *GetTypeInfo()      = 0;
            virtual void OnEnter(const C_FsmEvent *event) {}
            virtual void OnExit(const C_FsmEvent *event) {}
            virtual C_FsmState *ProcessEvent(const C_FsmEvent *event) = 0;
            virtual bool ShouldTickGame() {
                return false;
            }
            virtual void TickNoGame(const C_FsmEvent *event);
            virtual void TickGame(const C_FsmEvent *event);
            virtual void *GetHashName() = 0;

          public:
            void SetAvailableBudget(float budget) {
                _fAvailableBudget = budget;
            }

            float GetAvailableBudget() const {
                return _fAvailableBudget;
            }
        };

        class C_FsmState : public C_GameFSMState {};

        static_assert(sizeof(C_GameFSMState) == 0x78, "C_GameFSMState size mismatch");
        static_assert(sizeof(C_FsmState) == 0x78, "C_FsmState size mismatch");

    } // namespace ue
} // namespace SDK
