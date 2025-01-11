
#include "player.h"

#include "core/server.h"

#include "shared/rpc/chat_message.h"

namespace MafiaMP::Scripting {
    void Player::EventPlayerConnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();
        engine->InvokeEvent("onPlayerConnected", Player(e));
    }

    void Player::EventPlayerDisconnected(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();
        engine->InvokeEvent("onPlayerDisconnected", Player(e));
    }

    void Player::EventPlayerDied(flecs::entity e) {
        const auto engine = MafiaMP::Server::GetScriptingEngine();
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

    void Player::SendChat(std::string message) {
        const auto str = _ent.get<Framework::World::Modules::Base::Streamer>();
        FW_SEND_COMPONENT_RPC_TO(Shared::RPC::ChatMessage, SLNet::RakNetGUID(str->guid), message);
    }

    void Player::SendChatToAll(std::string message) {
        FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, message);
    }

    void Player::Register(sol::state &luaEngine) {
        sol::usertype<Player> cls = luaEngine.new_usertype<Player>("Player", sol::constructors<Player(uint64_t)>(), sol::base_classes, sol::bases<Human, Entity>());
        cls["destroy"]            = &Player::Destroy;
        cls["sendChat"]           = &Player::SendChat;
        cls["sendChatToAll"]      = &Player::SendChatToAll;
    }
} // namespace MafiaMP::Scripting
