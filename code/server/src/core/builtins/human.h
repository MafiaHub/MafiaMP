#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "entity.h"

#include <scripting/builtins/vector3.h>

#include "shared/modules/human_sync.hpp"

#include <fmt/format.h>
#include <logging/logger.h>

#include <memory>
#include <optional>

namespace MafiaMP::Scripting {
    class Vehicle;

    class Human: public Entity {
      public:
        Human(flecs::entity_t ent): Entity(ent) {
            const auto humanData = _ent.get<Shared::Modules::HumanSync::UpdateData>();

            if (!humanData) {
                throw std::runtime_error(fmt::format("Entity handle '{}' is not a Human!", ent));
            }
        }

        Human(flecs::entity ent): Human(ent.id()) {}

        std::string ToString() const override;

        bool IsAiming() const;

        bool IsFiring() const;

        void AddWeapon(int weaponId, int ammo);

        Framework::Scripting::Builtins::Vector3 GetAimDir() const;

        Framework::Scripting::Builtins::Vector3 GetAimPos() const;

        float GetHealth() const;
        void SetHealth(float health);

        std::string GetNickname() const;

        // Returns the Vehicle object, or undefined if not in a vehicle
        v8::Local<v8::Value> GetVehicle(v8::Isolate *isolate) const;

        // Returns the vehicle entity ID, or 0 if not in a vehicle
        uint64_t GetVehicleId() const;

        int GetVehicleSeatIndex() const;

        uint16_t GetWeaponId() const;

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Human> &GetClass(v8::Isolate *isolate);

      private:
        static std::unique_ptr<v8pp::class_<Human>> _class;
    };
} // namespace MafiaMP::Scripting
