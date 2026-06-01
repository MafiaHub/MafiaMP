#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include <scripting/builtins/quaternion.h>
#include <scripting/builtins/vector3.h>

#include <networking/replication/network_entity.h>

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace MafiaMP::Scripting {
    namespace Replication = Framework::Networking::Replication;

    // Base scripting handle for a replicated entity. Wraps the entity's NetworkID and resolves the
    // live NetworkEntity on demand through the ReplicationManager, so a stale JS handle to a
    // destroyed entity resolves to null instead of dangling.
    class Entity {
      public:
        Entity(uint64_t networkId);
        virtual ~Entity() = default;

        uint64_t GetId() const {
            return _id;
        }

        Replication::NetworkEntity *GetHandle() const {
            return Resolve();
        }

        Framework::Scripting::Builtins::Vector3 GetPosition() const;
        void SetPosition(const Framework::Scripting::Builtins::Vector3 &pos);

        Framework::Scripting::Builtins::Vector3 GetRotation() const;
        void SetRotationFromEuler(const Framework::Scripting::Builtins::Vector3 &rot);
        void SetRotationFromQuaternion(const Framework::Scripting::Builtins::Quaternion &quat);

        std::string GetModelName() const;

        virtual std::string ToString() const;

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Entity> &GetClass(v8::Isolate *isolate);

      protected:
        Replication::NetworkEntity *Resolve() const;

        uint64_t _id = 0;
        static std::unordered_map<v8::Isolate *, std::unique_ptr<v8pp::class_<Entity>>> _classes;
    };
} // namespace MafiaMP::Scripting
