#pragma once

#include <stdint.h>

namespace SDK {
    /**
     * Time budget information for tick management.
     * Used by the game FSM to track available time budget for frame processing.
     */
    class C_TimeBudgetInfo {
      public:
        float _fAvailableBudget; // 0x00 - Available time budget in seconds

      public:
        void SetAvailableBudget(float budget) {
            _fAvailableBudget = budget;
        }

        float GetAvailableBudget() const {
            return _fAvailableBudget;
        }

        /**
         * Context class for time budget management during tick processing.
         */
        class C_Ctx {
          private:
            uint64_t _unk0;              // 0x00
            uint32_t _unk8;              // 0x08
            int32_t _eventType;          // 0x0C - Event type (default: 5 = E_TMEVENT_TICK_ALWAYS)
            uint32_t _unk16;             // 0x10
            float _fMinBudget;           // 0x14 - Minimum budget
            C_TimeBudgetInfo *_pBudget;  // 0x18 - Pointer to time budget info
            uint32_t _unk32;             // 0x20
            uint32_t _unk36;             // 0x24
            uint64_t _unk40;             // 0x28
            uint8_t _pad48[32];          // 0x30

          public:
            C_Ctx()
                : _unk0(0)
                , _unk8(0)
                , _eventType(5)
                , _unk16(0)
                , _fMinBudget(0.0f)
                , _pBudget(nullptr)
                , _unk32(0)
                , _unk36(0)
                , _unk40(0)
                , _pad48 {0} {}

            void ForceMinBudget(float minBudget) {
                _fMinBudget = ((_fMinBudget - minBudget) < 0.0f) ? minBudget : _fMinBudget;
            }

            void BeginUpdate(C_TimeBudgetInfo *pTimeBudgetInfo = nullptr);
            void EndUpdate();
        };
    };
} // namespace SDK