#include "c_save_menu.h"

#include <utils/hooking/hooking.h>
#include "../../../patterns.h"

namespace SDK {
    namespace mafia::ui::menu {
        void C_MenuSave::OpenDebugLoadChapterString(ue::C_String &savePath, bool unk) {
            const auto C_MenuSave__OpenDebugLoadChapterStringAddr =
                reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B F9 4C 8B F2").get_first());
            hook::this_call(C_MenuSave__OpenDebugLoadChapterStringAddr, this, &savePath, unk);
        }
    } // namespace mafia::ui::menu
} // namespace SDK
