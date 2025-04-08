#include "c_injury_analyser.h"

#include "sdk/patterns.h"

namespace SDK {
    namespace ue::game::injury{
        void C_InjuryAnalyser::AddInjury(float hit, E_HitZone zone, sys::math::C_Vector const &pos) {
            hook::this_call(gPatterns.C_InjuryAnalyser__AddInjury, this, hit, zone, &pos);
        }
    }
}
