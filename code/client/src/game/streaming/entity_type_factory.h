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
        EntityTypeFactory(SpawnerCreateFn createSpawnerFn, SpawnerProcessLoadFn spawnerProcessLoadFn, SpawnerSpawnEntityFn spawnEntityFn,
            SpawnerReturnEntityFn returnEntityToSpawnerFn, SpawnerDestroyFn destroySpawnerFn)
            : _spawnerCreate(createSpawnerFn)
            , _spawnerProcessLoad(spawnerProcessLoadFn)
            , _spawnerSpawnEntity(spawnEntityFn)
            , _spawnerReturnEntity(returnEntityToSpawnerFn)
            , _spawnerDestroy(destroySpawnerFn) {}

        EntityTypeFactory(EntityTypeFactory &) = delete;
        EntityTypeFactory &operator=(EntityTypeFactory &) = delete;

        ~EntityTypeFactory() {
            ReturnAll();
            Update();
        }

        EntityTrackingInfo *Request(SDK::E_EntityType entityType, const Kind &entityKind) {
            // Acquire the spawner that must already exists
            SpawnerWrap *spawner = GetSpawner(entityKind);
            if (!spawner) {
                return nullptr;
            }

            // Acquire the informations
            ExtendedTrackingInfo extendedTrackingInfo(entityType, entityKind);
            EntityTrackingInfo *infos = extendedTrackingInfo.GetInfo().get();

            // Create an ongoing request to be processed at next tick
            spawner->_ongoingRequests.emplace_back(std::move(extendedTrackingInfo));

            // Store the actual used spawner
            _trackingInfos[infos] = spawner;
            return infos;
        }

        void Return(EntityTrackingInfo *infos) {
            // Make sure we find a matching entry before executing any process
            auto iter = _trackingInfos.find(infos);
            if (iter == _trackingInfos.end())
                return;

            EntityTypeFactory::SpawnerWrap *spawnerWrap = iter->second;

            // Only process if spawner instance is still valid
            if (spawner) {
                // If the entity hasn't been processed yet, remove it
                auto &ongoingRequests = spawnerWrap->_ongoingRequests;
                auto requestIter      = ongoingRequests.begin();
                while (requestIter != ongoingRequests.end()) {
                    if (requestIter->_info.get() == infos) {
                        if (infos->_return)
                            infos->_return(false);

                        requestIter = ongoingRequests.erase(requestIter);
                    }
                    else
                        ++requestIter;
                }

                // If the entity was processed, remove it
                auto &createdEntities = spawnerWrap->_createdEntities;
                auto entityIter       = createdEntities.begin();
                while (entityIter != createdEntities.end()) {
                    if (entityIter->GetInfo().get() == infos) {
                        if (infos->_return)
                            infos->_return(true);

                        _spawnerReturnEntity(spawnerWrap->GetInnerSpawner(), entityIter->GetInfo()->GetEntity());
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
            for (const auto &entry : _trackingInfos) trackedEntities.push_back(entry.first);

            for (EntityTrackingInfo *info : trackedEntities) Return(info);
        }

        void Update() {
            auto iter = _spawners.begin();
            while (iter != _spawners.end()) {
                // Do we have work to do in the backlog?
                if (iter->second._ongoingRequests.empty() || iter->second._createdEntities.empty()) {
                    EntityTypeFactory::SpawnerWrap &spawner = iter->second;

                    // If the spawner is still loading, don't do anything and wait
                    if (spawner.GetState() == EntitySpawnerState::Loading) {
                        spawner._state = _spawnerProcessLoad(spawner.GetInnerSpawner());
                    }
                    else {
                        auto &ongoingRequests = spawner._ongoingRequests;
                        auto request          = ongoingRequests.begin();

                        while (request != ongoingRequests.end()) {
                            bool requestFinished = false;

                            // If we have before spawn callback defined, call it
                            EntityTrackingInfo *infos = (*request).pInfo.get();
                            if (infos->_beforeSpawn) {
                                infos->_beforeSpawn();
                            }

                            // Call the entity spawn method
                            EntitySpawnResult spawnResult;
                            SDK::C_Entity *entity;
                            std::tie(spawnResult, entity) = _spawnerSpawnEntity(spawner.GetInnerSpawner());

                            // Depending on the result, push on list or not
                            if (spawnResult == EntitySpawnResult::OK) {
                                auto &createdEntities = spawner._createdEntities;
                                createdEntities.emplace_back(std::move(*request));
                                requestFinished = true;

                                EntityTrackingInfo *infos = _createdEntities.back().GetInfo().get();
                                infos->_entity            = entity;
                                if (infos->_requestFinish)
                                    infos->_requestFinish(true);
                            }
                            else if (spawnResult == EntitySpawnResult::Failed) {
                                requestFinished = true;

                                EntityTypeFactory::ExtendedTrackingInfo &request = *request;
                                request.GetInfo()->_entity                       = nullptr;
                                if (request.GetInfo()->_requestFinish)
                                    request.GetInfo()->_requestFinish(false);
                            }
                            else if (spawnResult == EntitySpawnResult::KeepLoading) {
                                spawner._state = EntitySpawnerState::Loading;
                            }

                            if (requestFinished)
                                request = ongoingRequests.erase(requestIter);
                            else
                                ++request;
                        }
                    }
                }
            }

            SpawnerWrap *GetSpawner(const Kind &) {
                auto iter = _spawners.find(kind);
                if (iter != _spawners.end())
                    return &iter->second;

                auto spawner = _spawnerCreate(kind);
                if (!spawner)
                    return nullptr;

                return &(*_spawners.emplace(kind, std::move(EntityTypeFactory::SpawnerWrap(spawner))).first).second;
            }
        }
    };
} // namespace MafiaMP::Game::Streaming