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
          private:
            uint64_t _unk0;              // 0000 - 0008
            uint32_t _unk8;              // 0008 - 000C
            int32_t _eventType;          // 000C - 0010
            uint32_t _unk16;             // 0010 - 0014
            float _fMinBudget;           // 0014 - 0018
            C_TimeBudgetInfo *_pBudget;  // 0018 - 0020
            uint32_t _unk32;             // 0020 - 0024
            uint32_t _unk36;             // 0024 - 0028
            uint64_t _unk40;             // 0028 - 0030
            uint8_t _pad48[32];          // 0030 - 0050

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
