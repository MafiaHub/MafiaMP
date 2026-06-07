#include "register_entities.h"

#include "human_entity.h"
#include "vehicle_entity.h"

#include <networking/replication/entity_factory.h>

namespace MafiaMP::Shared::Entities {
    void RegisterEntities() {
        auto &factory = Framework::Networking::Replication::EntityFactory::Get();
        factory.Register<HumanEntity>(HumanEntity::kTypeName);
        factory.Register<VehicleEntity>(VehicleEntity::kTypeName);
    }

    uint32_t HumanTypeId() {
        return Framework::Networking::Replication::EntityFactory::Get().TypeId(HumanEntity::kTypeName);
    }

    uint32_t VehicleTypeId() {
        return Framework::Networking::Replication::EntityFactory::Get().TypeId(VehicleEntity::kTypeName);
    }
} // namespace MafiaMP::Shared::Entities
