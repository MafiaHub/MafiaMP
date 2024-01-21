#include "c_save_menu.h"

#include "../../../patterns.h"

namespace SDK {
    namespace mafia::ui::menu {
        void C_MenuSave::OpenDebugLoadChapterString(ue::C_String &savePath, bool unk) {
            hook::this_call(gPatterns.C_MenuSave__OpenDebugLoadChapterString, this, &savePath, unk);
        }
    } // namespace mafia::ui::menu
} // namespace SDK
