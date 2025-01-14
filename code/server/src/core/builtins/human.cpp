#include "human.h"

#include "core/server.h"

#include "shared/game_rpc/human/human_add_weapon.h"
#include "shared/game_rpc/human/human_setprops.h"

#include "shared/modules/human_sync.hpp"

#include "vehicle.h"

namespace MafiaMP::Scripting {
    std::string Human::ToString() const {
        std::ostringstream ss;
        ss << "Human{ id: " << _ent.id() << " }";
        return ss.str();
    }

    void Human::AddWeapon(int weaponId, int ammo) {
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanAddWeapon, _ent, weaponId, ammo);
    }

    float Human::GetHealth() const {
        auto h = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        return h->_healthPercent;
    }

    void Human::SetHealth(float health) {
        auto h            = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        h->_healthPercent = health;
        MafiaMP::Shared::RPC::HumanSetProps msg {};
        msg.health = health;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
    }

    Vehicle Human::GetVehicle() const {
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return Vehicle(carEnt);
        }
        return Vehicle(-1);
    }

    int Human::GetVehicleSeatIndex() const {
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return updateData->carPassenger.seatId;
        }
        return -1;
    }

    void Human::Register(sol::state &luaEngine) {
        sol::usertype<Human> cls   = luaEngine.new_usertype<Human>("Human", sol::constructors<Human(uint64_t)>(), sol::base_classes, sol::bases<Entity>());
        cls["addWeapon"]           = &Human::AddWeapon;
        cls["getHealth"]           = &Human::GetHealth;
        cls["setHealth"]           = &Human::SetHealth;
        cls["getVehicle"]          = &Human::GetVehicle;
        cls["getVehicleSeatIndex"] = &Human::GetVehicleSeatIndex;
    }
} // namespace MafiaMP::Scripting
