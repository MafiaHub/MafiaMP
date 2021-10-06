#pragma once

#include "../../ue/c_cnt_ptr.h"

namespace SDK {
    enum class E_HumanMoveMode;
    class C_Actor;
    class I_Human2;

    class C_HumanScript {
      public:
        I_Human2 *m_pHuman;

      public:
        float GetHealth() const;
        void SetHealth(float health);

        float GetHealthMax() const;
        void SetHealthMax(float healthMax);

        bool IsDemigod() const;
        void SetDemigod(bool bDemigod);

        bool GetInvulnerabilityByScript() const;
        void SetInvulnerabilityByScript(bool bInvulnerabilityByScript);

        void GetOnVehicle(ue::C_CntPtr<uintptr_t> &outSyncObject, C_Actor *, unsigned int, bool, bool, E_HumanMoveMode moveMode, bool force);
        void GetOffVehicle(ue::C_CntPtr<uintptr_t> &outSyncObject, C_Actor *, bool, bool);
    };
}
