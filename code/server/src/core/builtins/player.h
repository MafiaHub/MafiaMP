#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "human.h"

#include <memory>

namespace MafiaMP::Scripting {
    class Player final: public MafiaMP::Scripting::Human {
      public:
        Player(flecs::entity_t ent): Human(ent) {}

        Player(flecs::entity ent): Human(ent.id()) {}

        static void EventPlayerConnected(flecs::entity e);

        static void EventPlayerDisconnected(flecs::entity e);

        static void EventPlayerDied(flecs::entity e);

        std::string ToString() const override;

        void Destroy();

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Player> &GetClass(v8::Isolate *isolate);

      private:
        static std::unique_ptr<v8pp::class_<Player>> _class;
    };
} // namespace MafiaMP::Scripting
