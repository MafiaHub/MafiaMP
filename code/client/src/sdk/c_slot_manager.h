#pragma once

#include "patterns.h"
#include "c_ticked_module.h"

namespace SDK {
    enum class E_SlotType : int {
        CITY_PART               = 1,
        CITY_GROUND             = 2,
        CITY_UNIVERSAL          = 3,
        CITY_SHOP               = 4,
        CHAR_UNIVERS            = 5,
        CHAR_PLAYER             = 6,
        CHAR_HI                 = 7,
        CHAR_LO                 = 8,
        CHAR_POLICE             = 9,
        CAR_UNIVERS             = 10,
        CAR_SMALL               = 11,
        CAR_BIG                 = 12,
        CAR_POLICE              = 13,
        CAR_SCRIPT              = 14,
        BASE_ANIM               = 15,
        WEAPONS                 = 16,
        GUI                     = 17,
        SKY                     = 18,
        TABLES                  = 19,
        SOUND_DEFAULT           = 20,
        PARTICLES               = 21,
        MUSIC                   = 22,
        GAME_SCRIPT             = 23,
        MISSION_SCRIPT          = 24,
        SCRIPT                  = 25,
        HI_CHAR_UNITED          = 26,
        LO_CHAR_UNITED          = 27,
        CAR_UNITED              = 28,
        POLICE_UNITED           = 29,
        TREES                   = 30,
        CITY_CRASH              = 31,
        GENERATE                = 32,
        SMALL                   = 33,
        SCRIPT_SOUNDS           = 34,
        LUA                     = 35,
        MAPA                    = 36,
        SOUND_CITY              = 37,
        ANIMS_CITY              = 38,
        KYN_CITY_PART           = 39,
        KYN_CITY_SHOP           = 40,
        GENERIC_SPEECH_NORMAL   = 41,
        GENERIC_SPEECH_GANGSTER = 42,
        GENERIC_SPEECH_VARIOUS  = 43,
        GENERIC_SPEECH_STORY    = 44,
        SCRIPT_UNITED           = 45,
        MISSION_SCRIPT_UNITED   = 46,
        GENERIC_SPEECH_POLICE   = 47,
        TEXT_DATABASE           = 48,
        WARDROBE                = 49,
        INGAME                  = 50,
        INGAME_GUI              = 51,
        DABING                  = 52,
        CAR_PAINTING            = 53,
        AP_DATA_SMALL           = 54,
        SANDBOX                 = 55,
        ROADMAP                 = 56,
        TEXT_DATABASE_CHAPTER   = 57,
        GUI2                    = 66,
        TEMP_SUBTITLES          = 67,
        PHONE                   = 68,
        GUI_IMAGES              = 69,
        PREGAME_GUI             = 71,
        SHARED_GUI              = 72,
        FONTS_EN_PC_GUI         = 73,
        FONTS_EN_PS4_GUI        = 74,
        FONTS_EN_XB1_GUI        = 75,
        FONTS_CHS_GUI           = 76,
        FONTS_CHT_GUI           = 77,
        FONTS_JP_GUI            = 78,
        FONTS_KR_GUI            = 79,
        FONTS_RU_CZ_GUI         = 80,
        LAST                    = 81
    };

    class C_SlotManager : public C_TickedModule {
      public:
        static C_SlotManager *GetInstance() {
            //NOTE: pattern here :)
            return *reinterpret_cast<C_SlotManager **>(0x000000014634F1B8);
        }
    };
} // namespace SDK
