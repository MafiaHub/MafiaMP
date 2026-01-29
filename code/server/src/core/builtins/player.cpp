#include "player.h"

#include "core/server.h"

#include "shared/rpc/chat_message.h"

#include <logging/logger.h>

namespace MafiaMP::Scripting {

std::unique_ptr<v8pp::class_<Player>> Player::_class;

void Player::EventPlayerConnected(flecs::entity e) {
    // TODO: Implement event dispatch via Framework::Scripting::Events::EmitGlobal()
    // The old InvokeGlobalEvent API no longer exists in the new V8 scripting system.
    // For now, this is a stub that can be enhanced later.
    Framework::Logging::GetLogger("Scripting")->debug("Player connected: {}", e.id());
}

void Player::EventPlayerDisconnected(flecs::entity e) {
    // TODO: Implement event dispatch via Framework::Scripting::Events::EmitGlobal()
    Framework::Logging::GetLogger("Scripting")->debug("Player disconnected: {}", e.id());
}

void Player::EventPlayerDied(flecs::entity e) {
    // TODO: Implement event dispatch via Framework::Scripting::Events::EmitGlobal()
    Framework::Logging::GetLogger("Scripting")->debug("Player died: {}", e.id());
}

std::string Player::ToString() const {
    std::ostringstream ss;
    ss << "Player{ id: " << _ent.id() << " }";
    return ss.str();
}

void Player::Destroy() {
    // Nothing should happen here, as the player entity is destroyed by the game and network systems
}

v8pp::class_<Player> &Player::GetClass(v8::Isolate *isolate) {
    if (!_class) {
        _class = std::make_unique<v8pp::class_<Player>>(isolate);
        _class->inherit<Human>()
            .ctor<flecs::entity_t>()
            .set("toString", &Player::ToString)
            .set("destroy", &Player::Destroy);
    }
    return *_class;
}

void Player::Register(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8pp::class_<Player> &cls = GetClass(isolate);
    auto ctx                  = isolate->GetCurrentContext();
    global->Set(ctx, v8pp::to_v8(isolate, "Player"), cls.js_function_template()->GetFunction(ctx).ToLocalChecked()).Check();
}

} // namespace MafiaMP::Scripting
