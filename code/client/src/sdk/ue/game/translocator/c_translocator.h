#pragma once

#include <cstdint>

#include "sdk/ue/c_time_budget.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    namespace ue::game::translocator {
        class I_Translocator {
          public:
            virtual ~I_Translocator() {};
        };

        class C_Translocator : public I_Translocator {
          public:
            struct S_ObjectToSpawn {
                void *pObject;                    // 0000 - 0008
                uint32_t index;                   // 0x08 - 000C
                uint32_t unk1;                    // 0x0C - 0010
                SDK::ue::sys::math::C_Vector pos; // 0x10 - 001C
            };

            void *m_pSpawner;                   // 0008 - 0010  (set by mov [rcx+8], rdx)
            void *unkPtr_10;                    // 0010 - 0018  (initialized to 0)
            void *m_pResourceActivatorList;     // 0018 - 0020  (set by mov [rcx+18h], r8)
            int32_t unk_20;                     // 0020 - 0024  (initialized to 0)
            char pad_24[4];                     // 0024 - 0028  (padding)
            void *unkPtr_28;                    // 0028 - 0030  (initialized to 0)
            uint8_t unkByte_30;                 // 0030 - 0031  (set to 1)
            char pad_31[3];                     // 0031 - 0034  (padding)
            float unkFloat_34;                  // 0034 - 0038  (set to 1.0f - 3F800000h)
            void *unkPtr_38;                    // 0038 - 0040  (initialized to 0)
            float unkFloat_40;                  // 0040 - 0044  (set to 1.0f - 3F800000h)
            char pad_44[4];                     // 0044 - 0048  (padding)
            S_ObjectToSpawn *objectBufferStart; // 0048 - 0050  (spawn buffer start)
            S_ObjectToSpawn *objectBufferCur;   // 0050 - 0058  (spawn buffer current)
            void *unkPtr_58;                    // 0058 - 0060  (possible buffer end?)
            void *unkPtr_60;                    // 0060 - 0068  (initialized to 0)
            void *unkPtr_68;                    // 0068 - 0070  (initialized to 0)
            void *unkPtr_70;                    // 0070 - 0078  (initialized to 0)
            void *unkPtr_78;                    // 0078 - 0080  (initialized to 0)
            void *unkPtr_80;                    // 0080 - 0088  (initialized to 0)
            void *unkPtr_88;                    // 0088 - 0090  (initialized to 0)
            void *unkPtr_90;                    // 0090 - 0098  (initialized to 0)
            void *unkPtr_98;                    // 0098 - 00A0  (initialized to 0)
            void *unkPtr_A0;                    // 00A0 - 00A8  (initialized to 0)

            // First array buffer
            char pad_A8[8];       // 00A8 - 00B0  (padding)
            void *buffer1Start;   // 00B0 - 00B8  (points to 0xC8)
            void *buffer1Current; // 00B8 - 00C0  (points to 0xC8)
            void *buffer1End;     // 00C0 - 00C8  (points to 0xC8 + 0x500)
            char buffer1[0x500];  // 00C8 - 05C8  (actual buffer space)

            // Second array buffer
            void *buffer2Start;   // 05C8 - 05D0  (points to 0x5E0)
            void *buffer2Current; // 05D0 - 05D8  (points to 0x5E0)
            void *buffer2End;     // 05D8 - 05E0  (points to 0x5E0 + 0x500)
            char buffer2[0x500];  // 05E0 - 0AE0  (actual buffer space)

            uint16_t unkFlags_AE0; // 0AE0 - 0AE2
            char pad_AE2[6];       // 0AE2 - 0AE8  (padding)
            void *unkPtr_AE8;      // 0AE8 - 0AF0  (from operator_new(0x60))
            void *unkPtr_AF0;      // 0AF0 - 0AF8  (initialized to 0)
            void *unkPtr_AF8;      // 0AF8 - 0B00  (from operator_new(0x60))
            void *unkPtr_B00;      // 0B00 - 0B08  (initialized to 0)
            void *unkPtr_B08;      // 0B08 - 0B10  (from operator_new(0x60))
            void *unkPtr_B10;      // 0B10 - 0B18  (initialized to 0)
            void *unkPtr_B18;      // 0B18 - 0B20  (initialized to 0)
            void *unkPtr_B20;      // 0B20 - 0B28  (initialized to 0)
            void *unkPtr_B28;      // 0B28 - 0B30  (initialized to 0)
            void *unkPtr_B30;      // 0B30 - 0B38  (initialized to 0)
            void *unkPtr_B38;      // 0B38 - 0B40  (initialized to 0)
            void *unkPtr_B40;      // 0B40 - 0B48  (initialized to 0)
            void *unkPtr_B48;      // 0B48 - 0B50  (initialized to 0)
            void *unkPtr_B50;      // 0B50 - 0B58  (initialized to 0)
            void *unkPtr_B58;      // 0B58 - 0B60  (initialized to 0)
            int32_t unkInt_B60;    // 0B60 - 0B64  (set to 1)
            int32_t unkInt_B64;    // 0B64 - 0B68  (set to 1)
            int32_t unkInt_B68;    // 0B68 - 0B6C  (set to 1)
            char pad_B6C[4];       // 0B6C - 0B70  (padding)

            // Third array buffer
            void *buffer3Start;   // 0B70 - 0B78  (points to 0xB88)
            void *buffer3Current; // 0B78 - 0B80  (points to 0xB88)
            void *buffer3End;     // 0B80 - 0B88  (points to 0xB88 + 0x500)
            char buffer3[0x500];  // 0B88 - 1088  (actual buffer space)

            uint8_t unkByte_1088; // 1088 - 1089
            char pad_1089[37];    // 1089 - 10C0  (padding)

            // Fourth array buffer
            void *buffer4Start;   // 10C0 - 10C8  (points to 0x10D8)
            void *buffer4Current; // 10C8 - 10D0  (points to 0x10D8)
            void *buffer4End;     // 10D0 - 10D8  (points to 0x10D8 + 0x500)
            char buffer4[0x500];  // 10D8 - 15E0  (actual buffer space)

            // Fifth array buffer
            void *buffer5Start;   // 15E0 - 15E8  (points to 0x15F8)
            void *buffer5Current; // 15E8 - 15F0  (points to 0x15F8)
            void *buffer5End;     // 15F0 - 15F8  (points to 0x15F8 + 0x500)
            char buffer5[0x500];  // 15F8 - 1AF8  (actual buffer space)

            // Sixth array buffer (smaller - 0x40 size)
            void *buffer6Start;   // 1AF8 - 1B00  (points to 0x1B10)
            void *buffer6Current; // 1B00 - 1B08  (points to 0x1B10)
            void *buffer6End;     // 1B08 - 1B10  (points to 0x1B10 + 0x40)
            char buffer6[0x40];   // 1B10 - 1B50  (actual buffer space)

            // Seventh array buffer (smaller - 0x40 size)
            void *buffer7Start;   // 1B50 - 1B58  (points to 0x1B68)
            void *buffer7Current; // 1B58 - 1B60  (points to 0x1B68)
            void *buffer7End;     // 1B60 - 1B68  (points to 0x1B68 + 0x40)
            char buffer7[0x40];   // 1B68 - 1BA8  (actual buffer space)

            // Eighth array buffer (smaller - 0x40 size)
            void *buffer8Start;   // 1BA8 - 1BB0  (points to 0x1BC0)
            void *buffer8Current; // 1BB0 - 1BB8  (points to 0x1BC0)
            void *buffer8End;     // 1BB8 - 1BC0  (points to 0x1BC0 + 0x40)
            char buffer8[0x40];   // 1BC0 - 1C00  (actual buffer space)

            int64_t unk_1C00;    // 1C00 - 1C08  (set to -1)
            void *unkPtr_1C08;   // 1C08 - 1C10  (initialized to 0)
            int32_t unkInt_1C10; // 1C10 - 1C14
            char pad_1C14[4];    // 1C14 - 1C18  (padding)
            void *unkPtr_1C18;   // 1C18 - 1C20  (initialized to 0)
            void *unkPtr_1C20;   // 1C20 - 1C28  (initialized to 0)
            void *unkPtr_1C28;   // 1C28 - 1C30  (initialized to 0)
            void *unkPtr_1C30;   // 1C30 - 1C38  (initialized to 0)

          public:
            void SpawnObject(S_ObjectToSpawn const& object);
            void SpawnObjects(ue::C_TimeBudget const &timeBudget);
        };

        static_assert(sizeof(C_Translocator::S_ObjectToSpawn) == 0x20, "C_Translocator::S_ObjectToSpawn is not the correct size");
    } // namespace ue::game::translocator
}
