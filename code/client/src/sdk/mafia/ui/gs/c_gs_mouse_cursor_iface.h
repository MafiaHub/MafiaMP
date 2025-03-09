#pragma once

namespace SDK {
    namespace mafia::ui::controllers {
        class C_MouseCursorController;
    }
    namespace mafia::ui::gs {
        class C_GSMouseCursorIface {
          public:
            char pad0[0x28];                                                            // 0000 - 0028
            mafia::ui::controllers::C_MouseCursorController *m_pMouseCursorController;  // 0028 - 0030

          public:
            void Hide();
            void Show();
        };
    }
}
