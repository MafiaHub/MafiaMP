#include "c_human_script.h"

#include <utils/hooking/hooking.h>

#include "../c_human_2.h"
#include "../../c_game.h"

#include "../../patterns.h"

namespace SDK {
    float C_HumanScript::GetHealth() const {
        return m_pHuman->m_fHealth;
    }

    void C_HumanScript::SetHealth(float health) {
        hook::this_call(gPatterns.C_HumanScript__SetHealth, this, health);
    }

    float C_HumanScript::GetHealthMax() const {
        if (m_pHuman->GetType() == E_EntityType::E_ENTITY_PLAYER) {
            C_Player2 *pActivePlayer = SDK::GetGame()->GetActivePlayer();
            uintptr_t pHealthSystem  = *(uintptr_t *)(((uintptr_t)pActivePlayer) + 3376);
            uintptr_t pHealthBar     = *(uintptr_t *)(pHealthSystem + 24);
            float fTotalHealth       = *(float *)(pHealthBar + 72);
            return fTotalHealth;
        }
        else {
            return m_pHuman->m_fHealthMax;
        }
    }

    void C_HumanScript::SetHealthMax(float healthMax) {
        if (m_pHuman->GetType() != E_EntityType::E_ENTITY_PLAYER) {
            m_pHuman->m_fHealthMax = healthMax;
        }
    }

    bool C_HumanScript::IsDemigod() const {
        return m_pHuman->m_bDemigod;
    }

    void C_HumanScript::SetDemigod(bool bDemigod) {
        m_pHuman->m_bDemigod = bDemigod;
    }

    bool C_HumanScript::GetInvulnerabilityByScript() const {
        return m_pHuman->m_bDemigod;
    }

    void C_HumanScript::SetInvulnerabilityByScript(bool bInvulnerabilityByScript) {
        m_pHuman->m_bInvulnerable = bInvulnerabilityByScript;
    }

    void C_HumanScript::GetOnVehicle(ue::C_CntPtr<uintptr_t> &outSyncObject, C_Actor *arg1, unsigned int arg2, bool arg3, bool arg4, E_HumanMoveMode moveMode, bool force) {
        (*(void(__thiscall *)(C_HumanScript *, ue::C_CntPtr<uintptr_t> &, C_Actor *, unsigned int, bool, bool, E_HumanMoveMode, bool))gPatterns.C_HumanScript__GetOnVehicle)(
            this, outSyncObject, arg1, arg2, arg3, arg4, moveMode, force);
    }

    void C_HumanScript::GetOffVehicle(ue::C_CntPtr<uintptr_t> &outSyncObject, C_Actor *arg1, bool arg2, bool arg3) {
        (*(void(__thiscall *)(C_HumanScript *, ue::C_CntPtr<uintptr_t> &, C_Actor *, bool, bool))gPatterns.C_HumanScript__GetOffVehicle)(this, outSyncObject, arg1, arg2, arg3);
    }
}
