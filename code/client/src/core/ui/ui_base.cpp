#include "ui_base.h"

#include "core/application.h"

namespace MafiaMP::Core::UI {
    bool UIBase::AreControlsLocked() const {
        if (gApplication->AreControlsLockedBypassed()) {
            return false;
        }

        return gApplication->AreControlsLocked();
    }

    void UIBase::LockControls(bool lock) const {
        gApplication->LockControls(lock);
    }
} // namespace MafiaMP::Core::UI
