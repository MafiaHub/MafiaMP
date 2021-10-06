#pragma once

#include <stdint.h>

namespace SDK {
    class C_TimeBudgetInfo {
      public:
        class C_Ctx {
          private:
            uint64_t m_Unk0;
            uint32_t m_Unk8;
            int m_Unk12;
            uint32_t m_Unk16;
            float m_Unk20;
            C_TimeBudgetInfo *m_Unk24;
            uint32_t m_Unk32;
            uint32_t m_Unk36;
            uint64_t m_Unk40;
            uint8_t m_Pad48[32 /*?*/];

          public:
            C_Ctx(): m_Unk0(0), m_Unk8(0), m_Unk12(5), m_Unk20(0.0f), m_Unk24(nullptr), m_Unk32(0), m_Unk36(0), m_Unk40(0), m_Pad48 {0} {}

            void ForceMinBudget(float a2) {
                m_Unk20 = (((m_Unk20 - a2) < 0.0f) ? a2 : m_Unk20);
            }

            void BeginUpdate(C_TimeBudgetInfo *pTimeBudgetInfo = nullptr);
            void EndUpdate();
        };
    };
} // namespace SDK