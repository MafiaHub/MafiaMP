#include "c_character_state_handler_move.h"

#include <sdk/patterns.h>
#include <sdk/entities/c_entity.h>
#include <sdk/entities/c_human_2.h>

namespace SDK {
    namespace ue::game::humanai {
        bool C_CharacterStateHandlerMove::IsSprinting() const {
            bool unk = (m_pCharacter->GetType() == E_EntityType::E_ENTITY_PLAYER ? *(float *)(((uintptr_t)m_pCharacter) + 0xC58) >= 0.0 : false);
            return m_pController->IsSprinting() && !m_pCharacter->IsCarryingBody() && !unk;
        }

        bool C_CharacterStateHandlerMove::SharpTurnTransitionActive() {
            return hook::this_call<bool>(gPatterns.C_CharacterStateHandlerMove__SharpTurnTransitionActive, this);
        }
    }
}
