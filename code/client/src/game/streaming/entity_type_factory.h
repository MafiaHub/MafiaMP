#pragma once

#include "sdk/entities/c_entity.h"

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

            ExtendedTrackingInfo(SDK::E_EntityType entityType, Kind entityKind): _kind(entityKind), _info(std::make_unique<EntityTrackingInfo>(entityType)) {}
            ExtendedTrackingInfo(ExtendedTrackingInfo &&other): _kind(other._kind), _info(std::exchange(other._info, nullptr)) {}
            ExtendedTrackingInfo(ExtendedTrackingInfo &)            = delete;
            ExtendedTrackingInfo &operator=(ExtendedTrackingInfo &) = delete;

            Kind GetKind() const {
                return _kind;
            }
        };

        struct SpawnerWrap {
            Spawner *_spawner         = nullptr;
            EntitySpawnerState _state = EntitySpawnerState::Loading;
            std::list<ExtendedTrackingInfo> _ongoingRequests;
            std::list<ExtendedTrackingInfo> _createdEntities;

            SpawnerWrap(Spawner *spawner): _spawner(spawner) {}
            SpawnerWrap(SpawnerWrap &&other): _spawner(std::exchange(other._spawner, nullptr)), _state(std::exchange(other._state, EntitySpawnerState::Loading)), _ongoingRequests(std::move(other._ongoingRequests)) {}
            SpawnerWrap(SpawnerWrap &)            = delete;
            SpawnerWrap &operator=(SpawnerWrap &) = delete;

            EntitySpawnerState GetState() const {
                return _state;
            }

            Spawner *GetInnerSpawner() const {
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
        EntityTypeFactory(SpawnerCreateFn createSpawnerFn, SpawnerProcessLoadFn spawnerProcessLoadFn, SpawnerSpawnEntityFn spawnEntityFn, SpawnerReturnEntityFn returnEntityToSpawnerFn, SpawnerDestroyFn destroySpawnerFn)
            : _spawnerCreate(createSpawnerFn)
            , _spawnerProcessLoad(spawnerProcessLoadFn)
            , _spawnerSpawnEntity(spawnEntityFn)
            , _spawnerReturnEntity(returnEntityToSpawnerFn)
            , _spawnerDestroy(destroySpawnerFn) {}

        EntityTypeFactory(EntityTypeFactory &)            = delete;
        EntityTypeFactory &operator=(EntityTypeFactory &) = delete;

        ~EntityTypeFactory() {
            ReturnAll();
            Update();
        }

        EntityTrackingInfo *Request(SDK::E_EntityType entityType, const Kind &entityKind) {
            SpawnerWrap *spawnerWrap = GetSpawner(entityKind);
            if (spawnerWrap) {
                ExtendedTrackingInfo extendedTrackingInfo(entityType, entityKind);
                EntityTrackingInfo *infos = extendedTrackingInfo._info.get();
                spawnerWrap->_ongoingRequests.emplace_back(std::move(extendedTrackingInfo));
                _trackingInfos[infos] = spawnerWrap;
                return infos;
            }

            return nullptr;
        }

        void Return(EntityTrackingInfo *infos) {
            // Make sure we find a matching entry before executing any process
            auto iter = _trackingInfos.find(infos);
            if (iter == _trackingInfos.end())
                return;

            EntityTypeFactory::SpawnerWrap *spawnerWrap = iter->second;

            // Only process if spawner instance is still valid
            if (spawnerWrap) {
                // If the entity hasn't been processed yet, remove it
                auto &ongoingRequests = spawnerWrap->_ongoingRequests;
                auto requestIter      = ongoingRequests.begin();
                while (requestIter != ongoingRequests.end()) {
                    if (requestIter->_info.get() == infos) {
                        if (infos->_return)
                            infos->_return(infos, false);

                        requestIter = ongoingRequests.erase(requestIter);
                    }
                    else
                        ++requestIter;
                }

                // If the entity was processed, remove it
                auto &createdEntities = spawnerWrap->_createdEntities;
                auto entityIter       = createdEntities.begin();
                while (entityIter != createdEntities.end()) {
                    if (entityIter->_info.get() == infos) {
                        if (infos->_return)
                            infos->_return(infos, true);

                        _spawnerReturnEntity(spawnerWrap->GetInnerSpawner(), entityIter->_info->GetEntity());
                        entityIter = createdEntities.erase(entityIter);
                    }
                    else
                        ++entityIter;
                }
            }

            _trackingInfos.erase(infos);
        }
        void ReturnAll() {
            std::vector<EntityTrackingInfo *> trackedEntities;
            trackedEntities.reserve(_trackingInfos.size());
            for (const auto &trackingInfoMapEntry : _trackingInfos) trackedEntities.push_back(trackingInfoMapEntry.first);

            for (EntityTrackingInfo *infos : trackedEntities) Return(infos);
        }

        // Update our collection of spawners
        // Each spawner can be in one of two states: Ready or Loading.
        // A spawner is Ready if it has finished loading and is now ready to spawn entities. A spawner is Loading if it is still loading and not ready to spawn entities.
        void Update() {
            auto spawnerIter = _spawners.begin();
            while (spawnerIter != _spawners.end()) {
                // Is there are any ongoing requests or is there are any entities that have been created?
                if (!spawnerIter->second._ongoingRequests.empty() || !spawnerIter->second._createdEntities.empty()) {
                    SpawnerWrap &spawner = spawnerIter->second;

                    if (spawner.GetState() == EntitySpawnerState::Ready) {
                        // Do we have ongoing requests ?
                        auto &ongoingRequests = spawner._ongoingRequests;
                        auto requestIter      = ongoingRequests.begin();
                        while (requestIter != ongoingRequests.end()) {
                            bool requestFinished = false;
                            {
                                EntityTrackingInfo *trackingInfo = (*requestIter)._info.get();
                                if (trackingInfo->_beforeSpawn)
                                    trackingInfo->_beforeSpawn(trackingInfo);
                            }
                            EntitySpawnResult spawnResult;
                            SDK::C_Entity *entity;
                            std::tie(spawnResult, entity) = _spawnerSpawnEntity(spawner.GetInnerSpawner());

                            // Entity was spawned successfully, update information and return
                            if (spawnResult == EntitySpawnResult::OK) {
                                auto &createdEntities = spawner._createdEntities;
                                createdEntities.emplace_back(std::move(*requestIter));
                                requestFinished = true;

                                EntityTrackingInfo *trackingInfo = createdEntities.back()._info.get();
                                trackingInfo->_entity            = entity;
                                if (trackingInfo->_requestFinish)
                                    trackingInfo->_requestFinish(trackingInfo, true);
                            }
                            else if (spawnResult == EntitySpawnResult::Failed) {
                                // Entity failed to spawn, return a null pointer
                                requestFinished = true;

                                ExtendedTrackingInfo &request = *requestIter;
                                request._info->_entity        = nullptr;
                                if (request._info->_requestFinish)
                                    request._info->_requestFinish(nullptr, false);
                            }
                            else if (spawnResult == EntitySpawnResult::KeepLoading) {
                                // If entity is still loading, flag the spawner as being loading
                                spawner._state = EntitySpawnerState::Loading;
                            }

                            if (requestFinished)
                                requestIter = ongoingRequests.erase(requestIter);
                            else
                                ++requestIter;
                        }
                    }
                    else if (spawner.GetState() == EntitySpawnerState::Loading) {
                        spawner._state = _spawnerProcessLoad(spawner.GetInnerSpawner());
                    }

                    ++spawnerIter;
                }
                else {
                    // Just destroy the spawner since there are no more job for it
                    _spawnerDestroy(spawnerIter->second.GetInnerSpawner());
                    spawnerIter = _spawners.erase(spawnerIter);
                }
            }
        }

        SpawnerWrap *GetSpawner(const Kind &kd) {
            auto iter = _spawners.find(kd);
            if (iter != _spawners.end())
                return &iter->second;

            auto spawner = _spawnerCreate(kd);
            if (!spawner)
                return nullptr;

            return &(*_spawners.emplace(kd, std::move(SpawnerWrap(spawner))).first).second;
        }
    };
} // namespace MafiaMP::Game::Streaming
