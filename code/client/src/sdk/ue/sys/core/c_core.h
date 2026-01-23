#pragma once

#include "../render/c_render_core.h"
#include "c_frame.h"
#include "c_resource_list.h"
#include "c_scene_object_manager.h"

namespace SDK {
    namespace ue::sys::core {
        class C_Core {
          public:
            virtual void *GetClassTypeInfo(void)                                                                                                              = 0;
            virtual void *GetTypeInfo(void)                                                                                                                   = 0;
            virtual unsigned int GetNumAdapters(void)                                                                                                         = 0;
            virtual void *GetAdapterDesc(unsigned int)                                                                                                        = 0;
            virtual unsigned int GetNumAvailableModes(unsigned int)                                                                                           = 0;
            virtual void *GetVideoModeDesc(unsigned int, unsigned int)                                                                                        = 0;
            virtual void *GetVideoModeIndexByResolution(unsigned int, unsigned int, unsigned int)                                                             = 0;
            virtual void *GetRecommendedGraphicsSettings(unsigned int, void *)                                                                                = 0;
            virtual void *GetGraphicsSettings(void)                                                                                                           = 0;
            virtual void SyncGraphicsSettings(void)                                                                                                           = 0;
            virtual void unk()                                                                                                                                = 0;
            virtual void Init()                                                                                                                               = 0;
            virtual void Done(void)                                                                                                                           = 0;
            virtual void PreShutdown(void)                                                                                                                    = 0;
            virtual bool IsInitialized(void)                                                                                                                  = 0;
            virtual void Reset()                                                                                                                              = 0;
            virtual void WaitForRenderFinished(void)                                                                                                          = 0;
            virtual unsigned int GetFrameNumber(void)                                                                                                         = 0;
            virtual void *GetTextureManager(void)                                                                                                             = 0;
            virtual void *GetBufferManager(void)                                                                                                              = 0;
            virtual void *GetMaterialManager(void)                                                                                                            = 0;
            virtual void *GetMFManager(void)                                                                                                                  = 0;
            virtual void *GetMemFileManager(void)                                                                                                             = 0;
            virtual void *GetXMLManager(void)                                                                                                                 = 0;
            virtual void *GetSceneManager(void)                                                                                                               = 0;
            virtual void *GetEnlightenSystem(void)                                                                                                            = 0;
            virtual void *GetEnlightenBuilder(void)                                                                                                           = 0;
            virtual void *GetPostProcessSetsManager(void)                                                                                                     = 0;
            virtual void *GetPaintManager(void)                                                                                                               = 0;
            virtual void *GetCorePaintTypes(void)                                                                                                             = 0;
            virtual C_ResourceList *GetResourceList(void)                                                                                                     = 0;
            virtual void *GetResourceActivatorList(void)                                                                                                      = 0;
            virtual C_SceneObjectManager *GetSceneObjectManager(void)                                                                                         = 0;
            virtual void *GetLoadSDSManager(void)                                                                                                             = 0;
            virtual float GetLodDistance()                                                                                                                    = 0;
            virtual void SetHDTextureLoader()                                                                                                                 = 0;
            virtual void RegisterFrameType()                                                                                                                  = 0;
            virtual void UnregisterFrameType(unsigned int)                                                                                                    = 0;
            virtual void *GetFrameTypeDesc(unsigned int)                                                                                                      = 0;
            virtual C_Frame *CreateFrame(unsigned int)                                                                                                        = 0;
            virtual void *GetRegisteredDescList(void)                                                                                                         = 0;
            virtual void AddPaintDataStoreResource()                                                                                                          = 0;
            virtual void RemovePaintDataStoreResource()                                                                                                       = 0;
            virtual void *GetPaintDataResource()                                                                                                              = 0;
            virtual void SwitchGenericResourceLoadMode(bool)                                                                                                  = 0;
            virtual void RegisterSyncManager()                                                                                                                = 0;
            virtual void UnregisterSyncManager()                                                                                                              = 0;
            virtual void Synchronize(void)                                                                                                                    = 0;
            virtual void GetSynchronizationTiming()                                                                                                           = 0;
            virtual void RegisterEndFrameCallback()                                                                                                           = 0;
            virtual void UnRegisterEndFrameCallback()                                                                                                         = 0;
            virtual void RegisterSyncCallback()                                                                                                               = 0;
            virtual void UnRegisterSyncCallback()                                                                                                             = 0;
            virtual void *CreateRenderDestination()                                                                                                           = 0;
            virtual void DestroyRenderDestination()                                                                                                           = 0;
            virtual void *GetRenderDestination(unsigned int)                                                                                                  = 0;
            virtual void *GetRenderDestinations(void)                                                                                                         = 0;
            virtual void BeginRenderFullscreenVideoMode(void)                                                                                                 = 0;
            virtual void EndRenderFullscreenVideoMode(void)                                                                                                   = 0;
            virtual bool IsFullScreenVideoModeActive(void)                                                                                                    = 0;
            virtual void BeginLoadingScreenMode()                                                                                                             = 0;
            virtual void EndLoadingScreenMode(void)                                                                                                           = 0;
            virtual unsigned int GetNumLoadingScreenImagesRendered(void)                                                                                      = 0;
            virtual void SetRenderPresentationMode(bool, bool)                                                                                                = 0;
            virtual void GetRenderPresentationMode(bool &)                                                                                                    = 0;
            virtual void RegisterExtendedModule()                                                                                                             = 0;
            virtual void UnRegisterExtendedModule()                                                                                                           = 0;
            virtual void RegisterRenderExtendedModule()                                                                                                       = 0;
            virtual void UnRegisterRenderExtendedModule()                                                                                                     = 0;
            virtual void RegisterShaderParamsProvider()                                                                                                       = 0;
            virtual void UnRegisterShaderParamsProvider()                                                                                                     = 0;
            virtual void RegisterCoreCallbacks()                                                                                                              = 0;
            virtual void UnRegisterCoreCallbacks()                                                                                                            = 0;
            virtual void SaveShaderCachesOnNextSync(bool)                                                                                                     = 0;
            virtual void LoadShaderCachesOnNextSync(void)                                                                                                     = 0;
            virtual void ReloadShaderSourcesChanges(void)                                                                                                     = 0;
            virtual void ReloadShaderSourceFile(char const *, bool)                                                                                           = 0;
            virtual unsigned int GetMainThreadId(void)                                                                                                        = 0;
            virtual void *GetSystemGraphicsDescriptor(void)                                                                                                   = 0;
            virtual void *GetSystemVideoModeDescriptor(void)                                                                                                  = 0;
            virtual void SetSystemVideoModeDescriptor()                                                                                                       = 0;
            virtual void Tick(float)                                                                                                                          = 0;
            virtual void RegisterForTick()                                                                                                                    = 0;
            virtual void UnregisterFromTick(unsigned long long)                                                                                               = 0;
            virtual void ProcessRegisteredTickFunctions(float)                                                                                                = 0;
            virtual void *GetImageFormatConvertor(void)                                                                                                       = 0;
            virtual float GetTime(void)                                                                                                                       = 0;
            virtual void *GetRenderConfig(void)                                                                                                               = 0;
            virtual void OnRenderConfigChanged(void)                                                                                                          = 0;
            virtual void SetFPSLimit(float)                                                                                                                   = 0;
            virtual void AsyncDeleteMeshData()                                                                                                                = 0;
            virtual void *GetSceneManagerPtr(void)                                                                                                            = 0;
            virtual void GetNeededLod()                                                                                                                       = 0;
            virtual void *GetSyncTarget(void)                                                                                                                 = 0;
            virtual void *GetSyncManager(void)                                                                                                                = 0;
            virtual void MarkObjectForSync(unsigned int)                                                                                                      = 0;
            virtual unsigned int GetSyncFlags(void)                                                                                                           = 0;
            virtual void AddSyncFlags(unsigned int)                                                                                                           = 0;

            char _pad0[0xD8];                             // 0x00 - 0xD8
            void *m_pFrameClassFactory;                   // 0xD8 - 0xE0
            void *m_pVertexFormatManager;                 // 0xE0 - 0xE8
            void *m_pMemFileManager;                      // 0xE8 - 0xF0
            void *m_pXmlManager;                          // 0xF0 - 0xF8
            char _pad1[0x08];                             // 0xF8 - 0x100
            C_SceneObjectManager *m_pSceneObjectManager;  // 0x100 - 0x108
            char _pad2[0xD8];                             // 0x108 - 0x1E0
            ue::sys::render::C_RenderCore *m_pRenderCore; // 0x1E0 - 0x1E8
        };
    } // namespace ue::sys::core
} // namespace SDK
