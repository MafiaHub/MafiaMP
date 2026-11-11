#pragma once

#include <scripting/class_cache.h>
#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/convert.hpp>

#include "shared/entities/human_entity.h"
#include "shared/outfits.h"

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

        // Spawn profile (skin): a decimal or 0x-prefixed hash, or a profile path such as
        // "visuals/hero/game_main_tommy_casual", hashed as the game does. False when the value is
        // invalid or the human is gone.
        bool SetSkin(const std::string &skin);
        // Dresses the human in the given pieces, on the template's body; returns the outfit's
        // spawn profile hash ("" if invalid).
        std::string SetOutfit(const std::vector<Shared::Outfits::Piece> &pieces, const std::string &templateProfile);
        // Current spawn profile hash, as a decimal string: it does not fit a JS number.
        std::string GetSkin() const;

        v8::Local<v8::Value> GetVehicle(v8::Isolate *isolate) const;
        uint64_t GetVehicleId() const;
        int GetVehicleSeatIndex() const;

        uint16_t GetWeaponId() const;

        static void Register(v8::Isolate *isolate, v8::Local<v8::Object> global);
        static v8pp::class_<Human> &GetClass(v8::Isolate *isolate);
        // Drops the isolate's class before the isolate is disposed.
        static void UnregisterIsolate(v8::Isolate *isolate);

      private:
        // One class per isolate: every resource runs in an isolate of its own.
        inline static Framework::Scripting::ClassCache<Human> _classes;
    };
} // namespace MafiaMP::Scripting
