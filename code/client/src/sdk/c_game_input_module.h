#pragma once

namespace SDK {
    class C_GameInputModule {
        public:
            void PauseInput(bool);
    };

    C_GameInputModule* GetGameInputModule();
}