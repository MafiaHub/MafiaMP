#include "entity_type_factory.h"

#include <tuple>

namespace MafiaMP::Game::Streaming {
    EntityTypeFactory::~EntityTypeFactory() {
        ReturnAll();
        Update();
    }

    EntityTrackingInfo *EntityTypeFactory::Request(SDK::E_EntityType entityType, const Kind &entityKind) {
        // Acquire the spawner that must already exists
        EntityTypeFactory::SpawnerWrap *spawner = GetSpawner(entityKind);
        if (!spawner) {
            return nullptr;
        }

        // Acquire the informations
        EntityTypeFactory::ExtendedTrackingInfo extendedTrackingInfo(entityType, entityKind);
        EntityTrackingInfo *infos = extendedTrackingInfo._info.get();

        // Create an ongoing request to be processed at next tick
        spawner->_ongoingRequests.emplace_back(std::move(extendedTrackingInfo));

        // Store the actual used spawner
        _trackingInfos[infos] = spawner;
        return infos;
    }

    void EntityTypeFactory::Return(EntityTrackingInfo *infos) {
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
                if (entityIter->_info.get() == infos) {
                    if (infos->_return)
                        infos->_return(true);

                    _spawnerReturnEntity(spawnerWrap->GetInnerSpawner(), entityIter->_info->GetEntity());
                    entityIter = createdEntities.erase(entityIter);
                }
                else
                    ++entityIter;
            }
        }

        _trackingInfos.erase(infos);
    }

    void EntityTypeFactory::ReturnAll() {
        std::vector<EntityTrackingInfo *> trackedEntities;
        trackedEntities.reserve(_trackingInfos.size());
        for (const auto &entry : _trackingInfos) trackedEntities.push_back(entry.first);

        for (EntityTrackingInfo *info : trackedEntities) Return(info);
    }

    void EntityTypeFactory::Update() {
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

        EntityTypeFactory::SpawnerWrap *EntityTypeFactory::GetSpawner(const Kind &) {
            auto iter = _spawners.find(kind);
            if (iter != _spawners.end())
                return &iter->second;

            auto spawner = _spawnerCreate(kind);
            if (!spawner)
                return nullptr;

            return &(*_spawners.emplace(kind, std::move(EntityTypeFactory::SpawnerWrap(spawner))).first).second;
        }
    }
} // namespace MafiaMP::Game::Streaming