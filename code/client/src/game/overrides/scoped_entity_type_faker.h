#pragma once

#include "../../sdk/entities/c_entity.h"

namespace MafiaMP::Game::Overrides {
    struct ScopedEntityTypeFaker {
      private:
        SDK::C_Entity *_entity;
        SDK::E_EntityType _origType;

      public:
        ScopedEntityTypeFaker(SDK::C_Entity *entity, SDK::E_EntityType fakeType): _entity(entity), _origType(entity->GetType()) {
            entity->SetType(fakeType);
        }
        ~ScopedEntityTypeFaker() {
            _entity->SetType(_origType);
        }
    };
}
