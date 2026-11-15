#pragma once

#include <scripting/class_cache.h>
#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include <scripting/builtins/vector3.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace MafiaMP::Scripting {
    // Script handle to a map blip. Blip.create() makes one; new Blip(id) wraps an existing id. The
    // blip lives on the server until destroy(), or until the resource that created it stops; a handle
    // to a destroyed blip reads as gone and ignores changes.
    class Blip final {
      public:
        Blip(uint32_t id);

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Blip> &GetClass(v8::Isolate *isolate);
        // Destroys the blips the isolate's scripts created and drops its class, before the isolate is
        // disposed.
        static void UnregisterIsolate(v8::Isolate *isolate);

        uint32_t GetId() const {
            return _id;
        }
        bool Exists() const;
        std::string ToString() const;

        Framework::Scripting::Builtins::Vector3 GetPosition() const;
        void SetPosition(const Framework::Scripting::Builtins::Vector3 &position);

        // The short name when the icon has one, otherwise the game's icon name.
        std::string GetIcon() const;
        void SetIcon(const std::string &icon);

        void Destroy();

      private:
        // Blip.create(position, icon = "generic", player = null)
        static void Create(const v8::FunctionCallbackInfo<v8::Value> &info);
        static void AttachTo(const v8::FunctionCallbackInfo<v8::Value> &info);
        static void ShowRoute(const v8::FunctionCallbackInfo<v8::Value> &info);
        static void HideRoute(const v8::FunctionCallbackInfo<v8::Value> &info);

        // The game's icon name for a blip icon: one of the short names, or a game icon name. Empty if
        // the name is not usable.
        static std::string ResolveIcon(const std::string &name);

        uint32_t _id = 0;

        // One class per isolate: every resource runs in an isolate of its own.
        inline static Framework::Scripting::ClassCache<Blip> _classes;
        // The blips each isolate's scripts created, destroyed with the isolate.
        inline static std::unordered_map<v8::Isolate *, std::unordered_set<uint32_t>> _created;
    };
} // namespace MafiaMP::Scripting
