#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "human.h"

#include <cstdint>
#include <memory>
#include <string>

namespace MafiaMP::Scripting {
    class Player final: public MafiaMP::Scripting::Human {
      public:
        Player(uint64_t networkId): Human(networkId) {}

        // The argument is the player's human NetworkID.
        static void EventPlayerConnected(uint64_t networkId);
        static void EventPlayerDisconnected(uint64_t networkId);
        static void EventPlayerDied(uint64_t networkId);

        std::string ToString() const override;

        void Destroy();
        void SendChat(std::string message);

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Player> &GetClass(v8::Isolate *isolate);

      private:
        static std::unique_ptr<v8pp::class_<Player>> _class;
    };
} // namespace MafiaMP::Scripting
