#pragma once

#include "c_ticked_module.h"

#include <stdint.h>

namespace SDK {
    enum E_TmEvent {
        E_TMEVENT_NAN                      = 0,
        E_TMEVENT_SYSTEM_INIT              = 1,
        E_TMEVENT_SYSTEM_DONE              = 2,
        E_TMEVENT_TICK                     = 3,
        E_TMEVENT_TICK_PAUSED              = 4,
        E_TMEVENT_TICK_ALWAYS              = 5,
        E_TMEVENT_TICK_GENERATOR           = 6,
        E_TMEVENT_RENDER                   = 7,
        E_TMEVENT_MISSION_QUIT             = 8,
        E_TMEVENT_GAME_INIT                = 9,
        E_TMEVENT_GAME_CLOSE               = 10,
        E_TMEVENT_GAME_DONE                = 11,
        E_TMEVENT_INVALIDATE_ENTITY        = 12,
        E_TMEVENT_INVALIDATE_FRAME         = 13,
        E_TMEVENT_WRITE_GAME_INFO          = 14,
        E_TMEVENT_READ_GAME_INFO           = 15,
        E_TMEVENT_GAME_RESTORE             = 16,
        E_TMEVENT_NOGAME_TICK              = 17,
        E_TMEVENT_NOGAME_RENDER            = 18,
        E_TMEVENT_NOGAME_AFTER_GAME_LOOP   = 19,
        E_TMEVENT_COLLISIONS_LOADED        = 20,
        E_TMEVENT_REG_GAME_SAVE_CB         = 21,
        E_TMEVENT_APP_DEACTIVATE           = 22,
        E_TMEVENT_APP_ACTIVATE             = 23,
        E_TMEVENT_LOADING_PROCESS_STARTED  = 24,
        E_TMEVENT_LOADING_PROCESS_FINISHED = 25,
        E_TMEVENT_GAME_PAUSED              = 26,
        E_TMEVENT_GAME_UNPAUSED            = 27,
        E_TMEVENT_TICK_SLOT_WAITING        = 28,
        E_TMEVENT_TICK_RENDER_WAITING      = 29,
        E_TMEVENT_SHUTDOWN                 = 30,
        E_TMEVENT_MAFIA_FIRST              = 31
    };

    struct I_TickedModuleCallEventContext {};

    using TickedModuleCallback = void (C_TickedModule::*)(I_TickedModuleCallEventContext &);
    class C_TickedModuleManager {
      public:
        virtual ~C_TickedModuleManager()                                                                                                    = default;
        virtual uint64_t GetClassTypeInfo()                                                                                                 = 0;
        virtual uint64_t GetTypeInfo()                                                                                                      = 0;
        virtual uint8_t AddEvent(unsigned int, char const *)                                                                                = 0;
        virtual uint64_t CallEvent(unsigned int, void const *, void *, bool)                                                                = 0;
        virtual uint64_t CallEvent(unsigned int, void *, void const *, void *, bool)                                                        = 0;
        virtual uint64_t CallEvents(void *STD2008Vector, void *C_TimeBudgetInfo, void const *, void *)                                      = 0;
        virtual uint64_t GetEventIdFromName(char const *)                                                                                   = 0;
        virtual uint64_t AddAction(E_TmEvent, int, C_TickedModule *, TickedModuleCallback, float, unsigned int, unsigned int, const char *) = 0;
        virtual uint64_t DelAction(E_TmEvent, C_TickedModule *, TickedModuleCallback)                                                       = 0;
        virtual uint64_t EnableAction(E_TmEvent, C_TickedModule *, TickedModuleCallback, bool)                                              = 0;
        virtual uint64_t RegisterStaticPlugins(void)                                                                                        = 0;
    };

    C_TickedModuleManager *GetTickedModuleManager();
} // namespace SDK