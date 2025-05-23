#include "c_streaming_traffic_module.h"

namespace SDK {
    namespace ue::game::traffic {
        void C_StreamingTrafficModule::CloseSeason(bool destroyActorItems) {
            hook::this_call<void>(gPatterns.C_StreamingTrafficModule__CloseSeason, this, destroyActorItems);
        }

        int C_StreamingTrafficModule::GetCurrentSeasonID() {
            return m_iSeasonID;
        }

        int C_StreamingTrafficModule::GetMaxHumanElements() {
            return m_iMaxHumanElements;
        }

        int C_StreamingTrafficModule::GetPreviousSeasonID() {
            return m_iPrevSeasonID;
        }

        bool C_StreamingTrafficModule::GetSeasonOpened() {
            return m_iSeasonID != -1;
        }

        void C_StreamingTrafficModule::OpenSeason(unsigned int seasonID, bool unk2) {
            hook::this_call<void>(gPatterns.C_StreamingTrafficModule__OpenSeason, this, seasonID, unk2);
        }

        void C_StreamingTrafficModule::SetMaxHumanElements(int maxHumanElements) {
            hook::this_call<void>(gPatterns.C_StreamingTrafficModule__SetMaxHumanElements, this, maxHumanElements);
        }
    } // namespace ue::game::traffic
} // namespace SDK
