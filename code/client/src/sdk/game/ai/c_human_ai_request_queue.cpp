#include "c_human_ai_request_queue.h"

#include "sdk/patterns.h"

namespace SDK {
    namespace game::ai {
        C_HumanAIRequestQueue::C_HumanAIRequestQueue(S_HumanAIRequest *request): _pad0 {}, m_nCurrentRequestIdx(1), _padC {}, m_RequestQueue {request} {}

        void C_HumanAIRequestQueue::GetRequestQueue(int32_t &count, ue::C_WeakPtr<S_HumanAIRequest, ue::S_SingleThreadedPolicy, 0> (&requests)[kMaxRequests]) {
            hook::this_call<void>(gPatterns.C_HumanAIRequestQueue__GetRequestQueue, this, &count, requests);
        }
    } // namespace game::ai
} // namespace SDK
