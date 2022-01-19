#pragma once

#include "../../sdk/entities/c_entity.h"

#include <functional>

#include <flecs/flecs.h>


namespace MafiaMP::Game::Streaming {
    class EntityTrackingInfo {
        template <typename, typename>
        friend class EntityTypeFactory;

      public:
        using BeforeSpawnCallback   = std::function<void(EntityTrackingInfo*)>;
        using RequestFinishCallback = std::function<void(EntityTrackingInfo*, bool)>;
        using ReturnCallback        = std::function<void(EntityTrackingInfo*, bool)>;
      private:
        SDK::E_EntityType _type;
        SDK::C_Entity *_entity = nullptr;

        BeforeSpawnCallback _beforeSpawn;
        RequestFinishCallback _requestFinish;
        ReturnCallback _return;
        flecs::entity _ecsEntity;

      public:
        EntityTrackingInfo(SDK::E_EntityType type) noexcept: _type(type) {};
        EntityTrackingInfo(EntityTrackingInfo &&other) noexcept: _type(other._type), _entity(std::exchange(_entity, nullptr)) {}
        EntityTrackingInfo(EntityTrackingInfo &) = delete;
        ~EntityTrackingInfo() {}

        EntityTrackingInfo &operator=(EntityTrackingInfo &) = delete;

        SDK::E_EntityType GetType() const {
            return _type;
        }

        SDK::C_Entity *GetEntity() const {
            return _entity;
        }
        flecs::entity GetNetworkEntity() const {
            return _ecsEntity;
        }

        bool IsCreated() const {
            return _entity != nullptr;
        }

        void SetBeforeSpawnCallback(BeforeSpawnCallback cb) {
            _beforeSpawn = cb;
        }
        void SetRequestFinishCallback(RequestFinishCallback cb) {
            _requestFinish = cb;
        }
        void SetReturnCallback(ReturnCallback cb) {
            _return = cb;
        }
        void SetNetworkEntity(flecs::entity ent) {
            _ecsEntity = ent;
        }
    };
} // namespace MafiaMP::Game::Streaming
