#pragma once

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "shared/entities/human_entity.h"

#include <scripting/builtins/player.h>
#include <scripting/builtins/vector3.h>

#include <cstdint>
#include <memory>
#include <string>

namespace MafiaMP::Scripting {
    class Human: public Framework::Scripting::Builtins::Player {
      public:
        Human(uint64_t networkId);

        // Resolves the handle as a HumanEntity, or nullptr if it is gone / not a human.
        Shared::Entities::HumanEntity *ResolveHuman() const;

        std::string ToString() const override;

        bool IsAiming() const;
        bool IsFiring() const;

        void AddWeapon(int weaponId, int ammo);

        Framework::Scripting::Builtins::Vector3 GetAimDir() const;
        Framework::Scripting::Builtins::Vector3 GetAimPos() const;

        float GetHealth() const;
        void SetHealth(float health);

        std::string GetNickname() const;

        v8::Local<v8::Value> GetVehicle(v8::Isolate *isolate) const;
        uint64_t GetVehicleId() const;
        int GetVehicleSeatIndex() const;

        uint16_t GetWeaponId() const;

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Human> &GetClass(v8::Isolate *isolate);

      private:
        static std::unique_ptr<v8pp::class_<Human>> _class;
    };
} // namespace MafiaMP::Scripting
