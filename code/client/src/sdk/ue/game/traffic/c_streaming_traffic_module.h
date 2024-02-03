#pragma once

#include "../../../c_ticked_module.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::traffic {
        class C_StreamingTrafficModule: public C_TickedModule {
          public:
            char pad0[0x8C];         // 0008 - 0094
            int m_SeasonID;          // 0094 - 0098
            int m_PrevSeasonID;      // 0098 - 009C
            char pad1[0x64];         // 009C - 0100
            int m_iMaxHumanElements; // 0100 - 0104

          public:
            /**
             * @param destroyActorItems see mafia::streaming::C_ActorsSlotWrapper::Close
             */
            void CloseSeason(bool destroyActorItems);

            /**
             * Custom method
             */
            int GetCurrentSeasonID();

            /**
             * Custom method
             */
            int GetMaxHumanElements();

            /**
             * Custom method
             */
            int GetPreviousSeasonID();

            /**
             * Check if any season is opened (seasonID != -1)
             */
            bool GetSeasonOpened();

            /**
             * If the current season is not -1, you must close the current season
             * with C_StreamingTrafficModule::CloseSeason before using it.
             */
            void OpenSeason(unsigned int seasonID, bool unk2);

            void SetMaxHumanElements(int maxHumanElements);

            static C_StreamingTrafficModule *GetInstance() {
                return hook::call<C_StreamingTrafficModule *>(gPatterns.C_StreamingTrafficModule__GetInstance);
            }
        };
    } // namespace ue::game::traffic
} // namespace SDK
