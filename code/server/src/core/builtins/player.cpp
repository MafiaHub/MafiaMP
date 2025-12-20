
#include "player.h"

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include <scripting/resource/resource_manager.h>

namespace MafiaMP::Scripting {
    void Player::EventPlayerConnected(flecs::entity e) {
        const auto resourceManager = Framework::CoreModules::GetResourceManager();
        if (resourceManager) {
            resourceManager->InvokeGlobalEvent("onPlayerConnected", Player(e));
        }
    }

    void Player::EventPlayerDisconnected(flecs::entity e) {
        const auto resourceManager = Framework::CoreModules::GetResourceManager();
        if (resourceManager) {
            resourceManager->InvokeGlobalEvent("onPlayerDisconnected", Player(e));
        }
    }

    void Player::EventPlayerDied(flecs::entity e) {
        const auto resourceManager = Framework::CoreModules::GetResourceManager();
        if (resourceManager) {
            resourceManager->InvokeGlobalEvent("onPlayerDied", Player(e));
        }
    }

    std::string Player::ToString() const {
        std::ostringstream ss;
        ss << "Player{ id: " << _ent.id() << " }";
        return ss.str();
    }

    void Player::Destroy() {
        // Nothing should happen here, as the player entity is destroyed by the game and network systems
    }

    void Player::Register(sol::state *luaEngine) {
        if (!luaEngine) {
            return;
        }

        sol::usertype<Player> cls = luaEngine->new_usertype<Player>("Player", sol::constructors<Player(uint64_t)>(), sol::base_classes, sol::bases<Human, Entity>());
        cls["__tostring"]         = &Player::ToString;
        cls["destroy"]            = &Player::Destroy;
    }
} // namespace MafiaMP::Scripting
