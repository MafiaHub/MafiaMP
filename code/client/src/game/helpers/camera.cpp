#include "camera.h"

#include <fmt/format.h>

#include "../../core/application.h"

namespace MafiaMP::Game::Helpers {
    bool Camera::SetPos(SDK::ue::sys::math::C_Vector start, SDK::ue::sys::math::C_Vector end, bool interpolate) {
        SDK::ue::sys::math::C_Vector finalVec = {start.x + end.x, start.y + end.y, start.z + end.z};
        const auto formattedVector            = fmt::format("Math:newVector({}, {}, {})", finalVec.x, finalVec.y, finalVec.z);
        const auto command                    = fmt::format("game.cameramanager:GetPlayerCamera():ScriptPointAtVec({}, 1, 1, 1)", formattedVector.c_str());
        return Core::gApplication->GetLuaVM()->ExecuteString(command.c_str());
    }

    bool Camera::ResetBehindPlayer() {
        return true;
    }

    bool Camera::SimpleShake() {
        return true;
    }
}
