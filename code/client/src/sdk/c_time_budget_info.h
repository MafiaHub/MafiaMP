#pragma once

#include <stdint.h>

namespace SDK {
    class C_TimeBudgetInfo {
      public:
        float _fAvailableBudget; // 0000 - 0004

      public:
        void SetAvailableBudget(float budget) {
            _fAvailableBudget = budget;
        }

        float GetAvailableBudget() const {
            return _fAvailableBudget;
        }

        class C_Ctx {
          public:
            float _fElapsedTime;         // 0000 - 0004
            float _fRequestedBudget;     // 0004 - 0008
            float _fRemainingBudget;     // 0008 - 000C
            int32_t _eventType;          // 000C - 0010
            uint32_t _unk10;             // 0010 - 0014
            float _fMinBudget;           // 0014 - 0018
            C_TimeBudgetInfo *_pBudget;  // 0018 - 0020
            uint32_t _unk20;             // 0020 - 0024
            uint32_t _unk24;             // 0024 - 0028
            uint64_t _timestamp;         // 0028 - 0030
            uint8_t _pad30[32];          // 0030 - 0050

          public:
            void ForceMinBudget(float minBudget) {
                _fMinBudget = ((_fMinBudget - minBudget) < 0.0f) ? minBudget : _fMinBudget;
            }

            void BeginUpdate(C_TimeBudgetInfo *pTimeBudgetInfo = nullptr);
            void EndUpdate();
        };
    };

    static_assert(sizeof(C_TimeBudgetInfo) == 0x04, "C_TimeBudgetInfo size mismatch");
    static_assert(sizeof(C_TimeBudgetInfo::C_Ctx) == 0x50, "C_TimeBudgetInfo::C_Ctx size mismatch");
} // namespace SDK
