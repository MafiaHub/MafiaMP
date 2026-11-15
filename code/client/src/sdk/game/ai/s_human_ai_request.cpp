#include "s_human_ai_request.h"

#include "c_human_ai_request_queue.h"

namespace SDK {
    namespace game::ai {
        ue::C_WeakPtr<S_HumanAIRequest, ue::S_SingleThreadedPolicy, 0> S_HumanAIRequest::GetWeakPtr() {
            // The game creates a request's weak proxy inline wherever it needs one; the request queue's
            // conversion is the only function that does it, so run it on a queue of this request alone.
            C_HumanAIRequestQueue queue(this);
            int32_t count = 0;
            ue::C_WeakPtr<S_HumanAIRequest, ue::S_SingleThreadedPolicy, 0> requests[C_HumanAIRequestQueue::kMaxRequests];
            queue.GetRequestQueue(count, requests);
            return requests[0];
        }
    } // namespace game::ai
} // namespace SDK
