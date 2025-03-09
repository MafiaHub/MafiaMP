#include "c_gs_mouse_cursor_iface.h"

#include "sdk/patterns.h"

namespace SDK {
    namespace mafia::ui::gs {
        void C_GSMouseCursorIface::Hide() {
            hook::this_call(gPatterns.C_GSMouseCursorIface__Hide, this);
        }
        void C_GSMouseCursorIface::Show() {
            hook::this_call(gPatterns.C_GSMouseCursorIface__Show, this);
        }
    }; // namespace mafia::ui::gs
} // namespace SDK
