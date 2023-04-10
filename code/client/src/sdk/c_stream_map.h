#pragma once

namespace SDK {
    class C_StreamMap {
      public:
        void CloseGame();
        void CloseMission();
        void ClosePart();
        const char *GetGame();
        const char *GetMission();
        const char *GetPart();
        void OpenGame(const char *);
        void OpenMission(const char *);
        void OpenPart(const char *);
    };
}
