#pragma once

#include <cstdint>
#include <vector>

#include "i_sds_resource.h"
#include "s_translocable_object_description.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_TranslocatorSDSResource : public I_SDSResource {
          public:
            virtual bool CreateResourceInstance(void *outInstance, void *loader, bool load) = 0;
            virtual bool DeleteResourceInstance(void *instance, void *loader, bool unload)  = 0;
            virtual bool IsStreamingReady()                                                 = 0;
            virtual bool Unk26()                                                            = 0;
            virtual bool Unk27()                                                            = 0;
            virtual bool Unk28()                                                            = 0;
            virtual bool Unk29()                                                            = 0;
            virtual bool Unk30()                                                            = 0;
            virtual bool Unk31()                                                            = 0;

            const std::vector<S_TranslocableObjectDescription> &GetObjectDescriptions() const {
                return m_ObjectDescriptions;
            }

            std::vector<S_TranslocableObjectDescription> &GetObjectDescriptions() {
                return m_ObjectDescriptions;
            }

            void *GetGlobalResource() const {
                return m_pGlobalResource;
            }

            const std::vector<void *> &GetLocalModifications() const {
                return m_LocalModifications;
            }

            const std::vector<void *> &GetLayerMetaData() const {
                return m_LayerMetaData;
            }

            void *GetTree() const {
                return m_pTree;
            }

          public:
            void *m_pGlobalResource;                                    // 0x48 - 0x50
            char _pad50[0x10];                                          // 0x50 - 0x60
            std::vector<S_TranslocableObjectDescription> m_ObjectDescriptions; // 0x60 - 0x78
            std::vector<void *> m_LocalModifications;                   // 0x78 - 0x90
            std::vector<void *> m_LayerMetaData;                        // 0x90 - 0xA8
            char m_CompressedDataConverter[0x58];                       // 0xA8 - 0x100
            void *m_pTree;                                              // 0x100 - 0x108
        };

        static_assert(sizeof(C_TranslocatorSDSResource) == 0x108, "C_TranslocatorSDSResource size mismatch");

    } // namespace ue::game::translocator
} // namespace SDK
