#pragma once

#include "../../../ue/c_string.h"
#include "c_menu_base.h"

namespace SDK {
    namespace mafia::ui::menu {
        class C_MenuSave: public C_MenuBase {
          public:
            void OpenDebugLoadChapterString(ue::C_String &savePath, bool unk);
        };
    } // namespace mafia::ui::menu
} // namespace SDK
