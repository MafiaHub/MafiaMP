#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::translocator {
        class I_SDSResource {
          public:
            virtual ~I_SDSResource()                                             = default;
            virtual void *GetClassTypeInfoVirt()                                 = 0;
            virtual void *GetClassTypeInfoVirt2()                                = 0;
            virtual void *GetTypeInfoVirt()                                      = 0;
            virtual void *GetTypeInfoVirt2()                                     = 0;
            virtual bool IsLoaded()                                              = 0;
            virtual bool IsLoadedVirt2()                                         = 0;
            virtual void SetLoaded(bool)                                         = 0;
            virtual void *GetResourceActivatorList()                             = 0;
            virtual I_SDSResource *GetParentResource()                           = 0;
            virtual uint16_t GetObjectCount()                                    = 0;
            virtual void *GetObjectByIndex(uint32_t index)                       = 0;
            virtual void *FindObjectByName(const char *name, uint32_t *outIndex) = 0;
            virtual uint16_t GetVirtualObjectCount()                             = 0;
            virtual void *GetVirtualObjectByIndex(uint32_t index)                = 0;
            virtual bool HasVirtualObjects()                                     = 0;
            virtual bool Unk16()                                                 = 0;
            virtual bool Unk17()                                                 = 0;
            virtual bool Unk18()                                                 = 0;
            virtual bool Unk19()                                                 = 0;
            virtual bool Unk20()                                                 = 0;
            virtual uint32_t GetTotalObjectCount()                               = 0;
            virtual void *Unk22()                                                = 0;

          protected:
            I_SDSResource *m_pParentResource; // 0x08 - 0x10
            char _pad10[0x38];                // 0x10 - 0x48
        };

        static_assert(sizeof(I_SDSResource) == 0x48, "I_SDSResource size mismatch");

    } // namespace ue::game::translocator
} // namespace SDK
