#pragma once

#include "integrations/server/scripting/builtins/node/entity.h"

namespace MafiaMP::Scripting {
    class Human: public Framework::Integrations::Scripting::Entity {
      private:
        bool _isPlayer = false;

      public:
        Human(flecs::entity_t ent, bool isPlayer = false): Entity(ent) {
            _isPlayer = isPlayer;
        }

        std::string ToString() const override;

        void Destroy(v8::Isolate *isolate);

        void AddWeapon(int weaponId, int ammo);

        v8::Local<v8::Value> GetVehicle(v8::Isolate *isolate) const;

        int GetVehicleSeat() const;

        float GetHealth() const;

        void SetHealth(float health);

        static void Register(v8::Isolate *isolate, v8pp::module *rootModule);

        static v8::Local<v8::Object> WrapHuman(v8::Isolate *isolate, flecs::entity e);
    };
} // namespace MafiaMP::Scripting
