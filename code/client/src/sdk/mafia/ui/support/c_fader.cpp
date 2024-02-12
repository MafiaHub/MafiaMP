#include "c_fader.h"

#include "../../../patterns.h"

namespace SDK {
    void mafia::ui::support::C_Fader::FadeIn(ue::C_CntPtr<uintptr_t> &outSyncObject, float unk1, char const *unk2, bool unk3) {
        hook::this_call(gPatterns.C_Fader__FadeIn, this, outSyncObject, unk1, unk2, unk3);
    }

    void mafia::ui::support::C_Fader::FadeOut(ue::C_CntPtr<uintptr_t> &outSyncObject, float unk1, char const *unk2, bool unk3) {
        hook::this_call(gPatterns.C_Fader__FadeOut, this, outSyncObject, unk1, unk2, unk3);
    }

    void mafia::ui::support::C_Fader::Reset() {
        hook::this_call(gPatterns.C_Fader__Reset, this);
    }
} // namespace SDK
