#include "c_fader.h"

#include <utils/hooking/hooking.h>

#include "../../../patterns.h"

namespace SDK {
    void mafia::ui::support::C_Fader::FadeIn(float unk1, char const* unk2, bool unk3) {
        hook::this_call(gPatterns.C_Fader__FadeIn, this, unk1, unk2, unk3);
    }

    void mafia::ui::support::C_Fader::FadeOut(float unk1, char const *unk2, bool unk3) {
        hook::this_call(gPatterns.C_Fader__FadeOut, unk1, unk2, unk3);
    }
}
