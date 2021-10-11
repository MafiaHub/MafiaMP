#pragma once

#include "../../sdk/entities/c_entity.h"
#include "../../sdk/mafia/streaming/c_actors_slot_wrapper.h"
#include "../../sdk/ue/game/traffic/c_human_spawner.h"
#include "../../sdk/ue/sys/utils/c_hash_name.h"
#include "entity_type_factory.h"

#include <string>
#include <functional>

/*namespace entity_factory_detail {
    inline void hash_combine(size_t &seed) {}

    template <typename T, typename... Rest>
    inline void hash_combine(size_t &seed, const T &v, Rest... rest) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, rest...);
    }
} // namespace entity_factory_detail

namespace std {
    template <>
    struct hash<VehicleKind> {
        size_t operator()(const VehicleKind &vehicle_kind) const {
            size_t hash = 0;
            entity_factory_detail::hash_combine(hash, static_cast<size_t>(vehicle_kind.first), vehicle_kind.second);
            return hash;
        }
    };
} // namespace std*/

namespace MafiaMP::Game::Streaming {
    class EntityFactory {
      private:
        EntityTypeFactory<SDK::ue::sys::utils::C_HashName, SDK::ue::game::traffic::C_HumanSpawner> _humanFactory;
        EntityTypeFactory<std::string, SDK::mafia::streaming::C_ActorsSlotWrapper> _treeFactory;
        EntityTypeFactory<std::string, SDK::mafia::streaming::C_ActorsSlotWrapper> _vehicleFactory;

      public:
        EntityFactory();

        EntityTrackingInfo *RequestHuman(SDK::ue::sys::utils::C_HashName);
        EntityTrackingInfo *RequestTree(const std::string &);
        EntityTrackingInfo *RequestVehicle(const std::string &);

        void Update();

        void ReturnEntity(EntityTrackingInfo *);
        void ReturnAll();
    };
} // namespace MafiaMP::Game::Streaming
