#pragma once

#include "c_character_state_handler.h"

namespace SDK {
    class C_Human2CarWrapper;
    namespace ue::game::humanai {
        class C_CharacterStateHandlerCar: public C_CharacterStateHandler {
          public:
            /*E_CAR_STATE*/ int GetCarState() const {
                return *(int *)(((uintptr_t)this) + 420);
            }

            C_Human2CarWrapper *GetHuman2CarWrapper() {
                ue::C_RefPtr<C_Human2CarWrapper> *refptr = (ue::C_RefPtr<C_Human2CarWrapper> *)(((uintptr_t)this) + 384);
                return refptr->ptr;
            }
        };
    }
}
