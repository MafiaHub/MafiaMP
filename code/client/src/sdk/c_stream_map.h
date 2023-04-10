#pragma once

namespace SDK {
    class C_StreamMap {
      private:
        void *pVFTable;                     // 0000 - 0008
        char pad0[0x2];                     // 0008 - 0010
        char *m_sGameName;                  // 0010 - 0018
        char *m_sMissionName;               // 0018 - 0020
        char *m_sPartName;                  // 0020 - 0028

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
