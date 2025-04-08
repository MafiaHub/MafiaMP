
#include "player.h"

#include "core/server.h"

#include "shared/rpc/chat_message.h"

namespace MafiaMP::Scripting {
    void Player::EventPlayerConnected(flecs::entity e) {
        const auto engine = Framework::CoreModules::GetScriptingEngine();
        engine->InvokeEvent("onPlayerConnected", Player(e));
    }

    void Player::EventPlayerDisconnected(flecs::entity e) {
        const auto engine = Framework::CoreModules::GetScriptingEngine();
        engine->InvokeEvent("onPlayerDisconnected", Player(e));
    }

    void Player::EventPlayerDied(flecs::entity e) {
        const auto engine = Framework::CoreModules::GetScriptingEngine();
        engine->InvokeEvent("onPlayerDied", Player(e));
    }

    std::string Player::ToString() const {
        std::ostringstream ss;
        ss << "Player{ id: " << _ent.id() << " }";
        return ss.str();
    }

    void Player::Destroy() {
        // Nothing should happen here, as the player entity is destroyed by the game and network systems
    }

    int Player::GetVehicleSeat() const {
        const auto updateData = _ent.get<MafiaMP::Shared::Modules::HumanSync::UpdateData>();
        const auto carEnt     = flecs::entity(_ent.world(), updateData->carPassenger.carId);
        if (carEnt.is_valid() && carEnt.is_alive()) {
            return updateData->carPassenger.seatId;
        }
        return -1;
    }

    /* Player NEEDS to be in a stream range of the vehicle for this action to work.
     * Suggestion: Teleport player to car position and then enforce entrance to ensure player always succeeds at this task.
     * Local player always waits for the vehicle to enter stream range, making the process entirely async, just like
     * how we handle remote human car enter/exit states.
     */
    bool Player::EnterVehicle(Vehicle vehicle, int seatId, bool forced) {
        if (!vehicle.GetHandle().is_valid())
            return false;
        if (!vehicle.GetHandle().is_alive())
            return false;
        if (seatId < 0 || seatId > 3 /* TODO: use MAX_SEATS constexpr var */)
            return false;
        auto updateData = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();

        if (updateData->carPassenger.carId)
            return false;

        {
            const auto carData = vehicle.GetHandle().get<Core::Modules::Vehicle::CarData>();
            if (!carData)
                return false;

            if (carData->seats[seatId])
                return false;
        }

        updateData->carPassenger.carId  = vehicle.GetID();
        updateData->carPassenger.seatId = seatId;
        MafiaMP::Shared::RPC::HumanSetProps msg {};
        MafiaMP::Shared::RPC::HumanSetProps::CarEnterExitAction carEnterExit = {};

        carEnterExit.carId     = vehicle.GetID();
        carEnterExit.seatId    = seatId;
        carEnterExit.forced    = forced;
        msg.carEnterExitAction = carEnterExit;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
        return true;
    }

    bool Player::ExitVehicle(bool forced) {
        auto updateData = _ent.get_mut<MafiaMP::Shared::Modules::HumanSync::UpdateData>();

        if (!updateData->carPassenger.carId)
            return false;

        updateData->carPassenger = {};
        MafiaMP::Shared::RPC::HumanSetProps msg {};
        MafiaMP::Shared::RPC::HumanSetProps::CarEnterExitAction carEnterExit = {};

        carEnterExit.forced    = forced;
        msg.carEnterExitAction = carEnterExit;
        FW_SEND_SERVER_COMPONENT_GAME_RPC(MafiaMP::Shared::RPC::HumanSetProps, _ent, msg);
        return true;
    }

    void Player::Register(sol::state *luaEngine) {
        if (!luaEngine) {
            return;
        }

        sol::usertype<Player> cls = luaEngine->new_usertype<Player>("Player", sol::constructors<Player(uint64_t)>(), sol::base_classes, sol::bases<Human, Entity>());
        cls["destroy"]            = &Player::Destroy;
        cls["getVehicle"]         = &Human::GetVehicle;
        cls["enterVehicle"]       = &Human::EnterVehicle;
        cls["exitVehicle"]        = &Human::ExitVehicle;
        cls["getVehicleSeat"]     = &Human::GetVehicleSeat;
    }
} // namespace MafiaMP::Scripting
