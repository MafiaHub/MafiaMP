#pragma once

#include <cstddef>
#include <cstdint>

#include "s_human_ai_request.h"

namespace SDK {
    namespace game::ai {
        class C_HumanAIRequestQueue {
          public:
            static constexpr int32_t kMaxRequests = 8;

            // A queue holding one request, without a reference to it, to run GetRequestQueue on.
            explicit C_HumanAIRequestQueue(S_HumanAIRequest *request);

            // Fills requests with weak pointers to the queued requests and clears (releasing) every slot
            // past them, so all kMaxRequests slots must exist.
            void GetRequestQueue(int32_t &count, ue::C_WeakPtr<S_HumanAIRequest, ue::S_SingleThreadedPolicy, 0> (&requests)[kMaxRequests]);

          private:
            char _pad0[0x8];                                // 0x00 - 0x08
            int32_t m_nCurrentRequestIdx;                   // 0x08 - 0x0C, the number of queued requests
            char _padC[0x4];                                // 0x0C - 0x10
            S_HumanAIRequest *m_RequestQueue[kMaxRequests]; // 0x10 - 0x50, TODO: ue::C_CntPtr<S_HumanAIRequest>[8] once the SDK's C_CntPtr has its real layout

            static void AssertLayout() {
                static_assert(sizeof(C_HumanAIRequestQueue) == 0x50, "C_HumanAIRequestQueue size mismatch");
                static_assert(offsetof(C_HumanAIRequestQueue, m_nCurrentRequestIdx) == 0x08, "C_HumanAIRequestQueue::m_nCurrentRequestIdx offset mismatch");
                static_assert(offsetof(C_HumanAIRequestQueue, m_RequestQueue) == 0x10, "C_HumanAIRequestQueue::m_RequestQueue offset mismatch");
            }
        };
    } // namespace game::ai
} // namespace SDK
