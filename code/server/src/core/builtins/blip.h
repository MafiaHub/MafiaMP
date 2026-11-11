#pragma once

#include <scripting/class_cache.h>
#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include <scripting/builtins/vector3.h>

#include <cstdint>
#include <string>

namespace MafiaMP::Scripting {
    // Script handle to a map blip. The blip lives on the server until destroy(); a handle to a
    // destroyed blip reads as gone and ignores changes.
    class Blip final {
      public:
        Blip(uint32_t id);

        // The game's icon name for a blip icon: one of the short names, or a game icon name.
        // Empty if the name is not usable.
        static std::string ResolveIcon(const std::string &name);

        // Resolves an Entity handle (player, human or vehicle) to its NetworkID; 0 for anything else.
        static uint64_t EntityId(v8::Isolate *isolate, v8::Local<v8::Value> value);

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Blip> &GetClass(v8::Isolate *isolate);
        // Drops the isolate's class before the isolate is disposed.
        static void UnregisterIsolate(v8::Isolate *isolate);

        uint32_t GetId() const {
            return _id;
        }
        bool Exists() const;
        std::string ToString() const;

        Framework::Scripting::Builtins::Vector3 GetPosition() const;
        void SetPosition(const Framework::Scripting::Builtins::Vector3 &position);

        std::string GetIcon() const;
        void SetIcon(const std::string &icon);

        void Destroy();

      private:
        uint32_t _id = 0;

        // One class per isolate: every resource runs in an isolate of its own.
        inline static Framework::Scripting::ClassCache<Blip> _classes;
    };
} // namespace MafiaMP::Scripting
