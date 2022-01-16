#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

static InitFunction init([]() {
    //Disable traffic
    const auto OpenSeasonAddr = hook::pattern("40 55 41 56 48 83 EC 48 83 B9 ? ? ? ? ?").get_first();
    hook::return_function(OpenSeasonAddr);
});
