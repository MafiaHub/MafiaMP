#pragma once

#include "../../sdk/entities/c_entity.h"
#include "entity_tracking_info.h"

#include <list>
#include <unordered_map>

namespace MafiaMP::Game::Streaming {
    enum class EntitySpawnerState { Loading, Ready };
    enum class EntitySpawnResult { KeepLoading, Failed, OK };

    template <typename Kind, typename Spawner>
    class EntityTypeFactory {
      public:
        using SpawnerCreateFn       = Spawner *(*)(const Kind &);
        using SpawnerProcessLoadFn  = EntitySpawnerState (*)(Spawner *);
        using SpawnerSpawnEntityFn  = std::pair<EntitySpawnResult, SDK::C_Entity *> (*)(Spawner *);
        using SpawnerReturnEntityFn = void (*)(Spawner *, SDK::C_Entity *);
        using SpawnerDestroyFn      = void (*)(Spawner *);

        struct ExtendedTrackingInfo {
            Kind _kind;
            std::unique_ptr<EntityTrackingInfo> _info;

            ExtendedTrackingInfo(E_EntityType entityType, Kind entityKind): _kind(entityKind), _info(std::make_unique<EntityTrackingInfo>(entityType)) {}
            ExtendedTrackingInfo(ExtendedTrackingInfo &&other): Kind(other.kind), pInfo(std::exchange(other.info, nullptr)) {}
            ExtendedTrackingInfo(ExtendedTrackingInfo &) = delete;
            ExtendedTrackingInfo &operator=(ExtendedTrackingInfo &) = delete;

            Kind GetKind() const {
              return _kind;
            }

            std::unique_ptr<EntityTrackingInfo> GetInfo() const {
              return _info;
            }
        };

        struct SpawnerWrap {
            Spawner *_spawner         = nullptr;
            EntitySpawnerState _state = EntitySpawnerState::Loading;
            std::list<ExtendedTrackingInfo> _ongoingRequests;
            std::list<ExtendedTrackingInfo> _createdEntities;

            SpawnerWrap(Spawner *spawner): _spawner(spawner) {}
            SpawnerWrap(SpawnerWrap &&other)
                : _spawner(std::exchange(other._spawner, nullptr))
                , _state(std::exchange(other._state, EntitySpawnerState::Loading))
                , _ongoingRequests(std::move(other._ongoingRequests)) {}
            SpawnerWrap(SpawnerWrap &) = delete;
            SpawnerWrap &operator=(SpawnerWrap &) = delete;

            EntitySpawnerState GetState() const {
              return _state;
            }

            Spawner* GetInnerSpawner() const {
              return _spawner;
            }
        };

      protected:
        SpawnerCreateFn _spawnerCreate;
        SpawnerProcessLoadFn _spawnerProcessLoad;
        SpawnerSpawnEntityFn _spawnerSpawnEntity;
        SpawnerReturnEntityFn _spawnerReturnEntity;
        SpawnerDestroyFn _spawnerDestroy;
        std::unordered_map<Kind, SpawnerWrap> _spawners;
        std::unordered_map<EntityTrackingInfo *, SpawnerWrap *> _trackingInfos;

      public:
        EntityTypeFactory(SpawnerCreateFn createSpawnerFn, SpawnerProcessLoadFn spawnerProcessLoadFn, SpawnerSpawnEntityFn spawnEntityFn,
            SpawnerReturnEntityFn returnEntityToSpawnerFn, SpawnerDestroyFn destroySpawnerFn)
            : _spawnerCreate(createSpawnerFn)
            , _spawnerProcessLoad(spawnerProcessLoadFn)
            , _spawnerSpawnEntity(spawnEntityFn)
            , _spawnerReturnEntity(returnEntityToSpawnerFn)
            , _spawnerDestroy(destroySpawnerFn) {}

        EntityTypeFactory(EntityTypeFactory &) = delete;
        EntityTypeFactory &operator=(EntityTypeFactory &) = delete;

        ~EntityTypeFactory();

        EntityTrackingInfo *Request(SDK::E_EntityType, const Kind &);

        void Return(EntityTrackingInfo *);
        void ReturnAll();

        void Update();

        SpawnerWrap *GetSpawner(const Kind &);
    };
} // namespace MafiaMP::Game::Streaming