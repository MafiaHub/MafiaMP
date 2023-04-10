#pragma once

namespace SDK {
    class C_StreamMap {
      private:
        void *pVFTable;
        char pad0[0x2];
        char *m_sGameName;
        char *m_sMissionName;
        char *m_sPartName;

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
