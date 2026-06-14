#pragma once

#include "c_actor.h"

#include "../ue/game/crashobject/c_deformation.h"
#include "../ue/sys/math/c_matrix.h"

#include <cstdint>
#include <vector>

namespace SDK {
    // Size: 0x470 (1136 bytes)
    class C_ActorDeform : public C_Actor {
      public:
        ue::game::crashobject::C_Deformation *GetDeformation() {
            return reinterpret_cast<ue::game::crashobject::C_Deformation *>((uintptr_t)this + 0xF8);
        }

      protected:
        char m_deformObject[0x2E0];                         // 0xF8 - 0x3D8 (embedded deformation object, extends C_Deformation with interface vtables at +0x248, +0x250, +0x258)
        uint32_t m_nDeformFlags;                            // 0x3D8 - 0x3DC (bitmasks: 0x200 owner info, 0x4000000 transform dirty, 0x10100000 pending update, 0x20000000 boxes)
        uint8_t _pad3DC[0x4];                               // 0x3DC - 0x3E0
        std::vector<void *> m_aDeformParts;                 // 0x3E0 - 0x3F8 (80-byte elements)
        uint32_t m_nUnk3F8;                                 // 0x3F8 - 0x3FC
        uint8_t m_bUnk3FC;                                  // 0x3FC - 0x3FD
        uint8_t _pad3FD[0x3];                               // 0x3FD - 0x400
        ue::sys::math::C_Matrix m_matrix;                   // 0x400 - 0x430 (initialized to identity)
        void *m_pSubdivObj;                                 // 0x430 - 0x438 (ref-counted subdiv object)
        std::vector<void *> m_aUnk438;                      // 0x438 - 0x450 (64-byte elements)
        std::vector<uint32_t> m_aUnk450;                    // 0x450 - 0x468 (4-byte elements)
        uint16_t m_nUnk468;                                 // 0x468 - 0x46A
        uint8_t _pad46A[0x6];                               // 0x46A - 0x470
    };

    static_assert(sizeof(C_ActorDeform) == 0x470, "C_ActorDeform size mismatch");
} // namespace SDK
