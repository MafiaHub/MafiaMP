#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue {
        class C_FsmEvent {
          public:
            void *_pUnknown08;  // 0008 - 0010
            int32_t _eventType; // 0010 - 0014
            int32_t _pad14;     // 0014 - 0018

          public:
            virtual ~C_FsmEvent() = default;

            int32_t GetEventType() const {
                return _eventType;
            }
        };

        struct S_GameFSMEventContext {
            void *_pTickInfo; // 0000 - 0008
        };

        class C_TickEvent : public C_FsmEvent {
          public:
            S_GameFSMEventContext *_pContext; // 0018 - 0020
            float _fDeltaTime;                // 0020 - 0024
            int32_t _iTickInfo1;              // 0024 - 0028
            int32_t _iTickInfo2;              // 0028 - 002C
            int32_t _iTickFlags;              // 002C - 0030
            int32_t *_pFrameSkipCounter;      // 0030 - 0038

          public:
            float GetDeltaTime() const {
                return _fDeltaTime;
            }

            S_GameFSMEventContext *GetContext() const {
                return _pContext;
            }
        };

        static_assert(sizeof(C_FsmEvent) == 0x18, "C_FsmEvent size mismatch");
        static_assert(sizeof(C_TickEvent) == 0x38, "C_TickEvent size mismatch");

    } // namespace ue
} // namespace SDK
