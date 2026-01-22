#pragma once

namespace SDK {
    namespace mafia::health_system {

        class I_HealthBarListener {
          public:
            virtual ~I_HealthBarListener() = default;
            virtual void OnExitDangerZone()    = 0; // vtable 0x00
            virtual void OnEnterDangerZone()   = 0; // vtable 0x08
            virtual void OnFullyHealed()       = 0; // vtable 0x10
            virtual void OnStartRegeneration() = 0; // vtable 0x20
            virtual void OnStopRegeneration()  = 0; // vtable 0x28
            virtual void OnDeath()             = 0; // vtable 0x38
        };

    } // namespace mafia::health_system
} // namespace SDK
