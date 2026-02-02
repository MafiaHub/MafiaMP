#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include <scripting/builtins/quaternion.h>
#include <scripting/builtins/vector3.h>

#include <flecs.h>
#include <world/modules/base.hpp>

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace MafiaMP::Scripting {

    /**
     * Base Entity class for MafiaMP scripting.
     * Provides common entity functionality accessible from JavaScript.
     */
    class Entity {
      public:
        Entity(flecs::entity_t ent);
        Entity(flecs::entity ent): Entity(ent.id()) {}
        virtual ~Entity() = default;

        uint64_t GetId() const {
            return _ent.id();
        }

        flecs::entity GetHandle() const {
            return _ent;
        }

        // Property accessors (used by v8 SetAccessor)
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
        flecs::entity _ent;
        static std::unordered_map<v8::Isolate*, std::unique_ptr<v8pp::class_<Entity>>> _classes;
    };

} // namespace MafiaMP::Scripting
