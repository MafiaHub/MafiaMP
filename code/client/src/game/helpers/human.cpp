#include "human.h"

#include "../../sdk/entities/c_human_2.h"
#include "../../sdk/entities/human/c_human_script.h"

namespace MafiaMP::Game::Helpers {
    uint8_t Human::GetHealthPercent(SDK::C_Human2 *human){
        float fHealth        = human->GetHumanScript()->GetHealth();
        float fHealthMax     = human->GetHumanScript()->GetHealthMax();
        float fHealthPercent = std::clamp((fHealth / fHealthMax) * 100.f, 0.0f, 100.0f);
        return (uint8_t)fHealthPercent;
    }
}
