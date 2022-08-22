#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

static InitFunction init([]() {
    // These patches are disabled since we want vehicles in debug mode hihi
    
    // Disable automated vehicle enable (engine, siren, beacon lights etc...) when player enter it
    const auto human2CarWrapperStartDrivePattern = hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 57 48 83 EC 70 45 33 FF").get_first();
    // hook::return_function(human2CarWrapperStartDrivePattern);

    // Disable automated vehicle disable (engine, siren, beacon lights etc...) when player leaves it
    const auto human2CarWrapperEndDrivePattern = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 08 48 8B 7C 24 ? ");
    // hook::return_function(human2CarWrapperEndDrivePattern);
});
