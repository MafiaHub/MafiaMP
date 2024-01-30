#pragma once

#include "../../../c_ticked_module.h"

#include "../../../patterns.h"

namespace SDK {
    namespace ue::game::traffic {
        class C_StreamingTrafficModule: public C_TickedModule {
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

            /**
             * @returns C_TrafficSpawnManager::UpdateMaxElementsCnt value
             */
            int SetMaxHumanElements(int maxHumanElements);

            static C_StreamingTrafficModule *GetInstance() {
                return hook::call<C_StreamingTrafficModule *>(gPatterns.C_StreamingTrafficModule__GetInstance);
            }

          private:
            char pad_0094[0x8C];     // 0008 - 0094
            int m_SeasonID;          // 0094 - 0098
            int m_PrevSeasonID;      // 0098 - 009C
            char pad_009C[0x19C];    // 009C - 100
            int m_iMaxHumanElements; // 100 - 104
        };
    } // namespace ue::game::traffic
} // namespace SDK
