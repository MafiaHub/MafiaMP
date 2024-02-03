#include "c_human_2_car_wrapper.h"

#include "../patterns.h"

namespace SDK {
    bool C_Human2CarWrapper::IsDriver(C_Actor *pActor) {
        return GetSeatID(pActor) == 0;
    }

    bool C_Human2CarWrapper::IsEngineOn() {
        return hook::this_call<bool>(gPatterns.C_Human2CarWrapper__IsEngineOn, this);
    }

    unsigned int C_Human2CarWrapper::GetSeatID(C_Actor *pActor) {
        return hook::this_call<unsigned int>(gPatterns.C_Human2CarWrapper__GetSeatID, this, pActor);
    }
} // namespace SDK
