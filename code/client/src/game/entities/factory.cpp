#include "factory.h"

namespace MafiaMP::Game::Entities {
    SDK::ue::sys::core::C_Frame* AllocateFrame() {
        SDK::ue::sys::core::C_Frame *frameToClone = nullptr;
        // TODO: grab from slot

        auto outModel = SDK::ue::sys::core::I_Core::GetInstance()->CreateFrame(2);
        if (!outModel) {
            return nullptr;
        }

        outModel->DuplicateV(frameToClone);
        return outModel;
    }
}
