#pragma once

namespace ue::game::traffic {
    class C_StreamingTrafficModule {
        public:
            void SetMaxHumanElements(int64_t);

        static C_StreamingTrafficModule *GetInstance();
    }
}