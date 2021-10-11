#pragma once

#include "../../sdk/entities/c_entity.h"

#include <functional>

namespace MafiaMP::Game::Streaming {
    using BeforeSpawnCallback   = std::function<void()>;
    using RequestFinishCallback = std::function<void(bool)>;
    using ReturnCallback        = std::function<void(bool)>;

    class EntityTrackingInfo {
      private:
        SDK::E_EntityType _type;
        SDK::C_Entity *_entity;

        BeforeSpawnCallback _beforeSpawn;
        RequestFinishCallback _requestFinish;
        ReturnCallback _return;

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
    };
} // namespace MafiaMP::Game::Streaming
