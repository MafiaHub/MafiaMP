#pragma once

#include "../render/c_render_core.h"

namespace SDK {
    namespace ue::sys::core {
        class C_Core {
          public:
            // virtual ~C_Core() = 0;
            virtual void *GetClassTypeInfo(void)                                                                                                              = 0;
            virtual void *GetTypeInfo(void)                                                                                                                   = 0;
            virtual unsigned int GetNumAdapters(void)                                                                                                         = 0;
            virtual void *GetAdapterDesc(unsigned int)                                                                                                        = 0;
            virtual unsigned int GetNumAvailableModes(unsigned int)                                                                                           = 0;
            virtual void *GetVideoModeDesc(unsigned int, unsigned int)                                                                                        = 0;
            virtual void *GetVideoModeIndexByResolution(unsigned int, unsigned int, unsigned int)                                                             = 0;
            virtual void *GetRecommendedGraphicsSettings(/*ue::sys::render::E_GraphicQuality*/ unsigned int, /*ue::sys::render::S_GraphicSettings &*/ void *) = 0;
            virtual void *GetGraphicsSettings(void)                                                                                                           = 0;
            virtual void SyncGraphicsSettings(void)                                                                                                           = 0;
            virtual void unk()                                                                                                                                = 0;
            virtual void Init(/*ue::sys::render::S_GraphicsInitDesc const&*/) = 0;
            virtual void Done(void)                                                                                                                           = 0;
            virtual void PreShutdown(void)                                                                                                                    = 0;
            virtual bool IsInitialized(void)                                                                                                                  = 0;
            virtual void Reset(/*ue::sys::render::S_GraphicsInitDesc const&*/) = 0;
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
            virtual void *GetResourceList(void)                                                                                                               = 0;
            virtual void *GetResourceActivatorList(void)                                                                                                      = 0;
            virtual void *GetSceneObjectManager(void)                                                                                                         = 0;
            virtual void *GetLoadSDSManager(void)                                                                                                             = 0;
            /*virtual void* GetRenderCore(void) = 0;
            virtual void* GetPresentListener(void) = 0;
            virtual void* GetVertexFormatManager(void) = 0;*/
            virtual float GetLodDistance(/*ue::sys::core::C_SceneObject const**/)                                                                                   = 0;
            virtual void SetHDTextureLoader(/*ue::C_Ptr<ue::sys::core::I_HDTextureLoader>*/) = 0;
            virtual void RegisterFrameType(/*ue::sys::core::I_FrameTypeDesc **/) = 0;
            virtual void UnregisterFrameType(unsigned int)                                                                                                          = 0;
            virtual void *GetFrameTypeDesc(unsigned int)                                                                                                            = 0;
            virtual C_Frame *CreateFrame(unsigned int)                                                                                                              = 0;
            virtual void *GetRegisteredDescList(void)                                                                                                               = 0;
            virtual void AddPaintDataStoreResource(/*ue::C_Ptr<ue::sys::core::S_PaintDataStore2DResource>*/) = 0;
            virtual void RemovePaintDataStoreResource(/*ue::sys::utils::C_HashName &*/) = 0;
            virtual void *GetPaintDataResource(/*ue::sys::utils::C_HashName &*/) = 0;
            virtual void SwitchGenericResourceLoadMode(bool)                                                                                                        = 0;
            virtual void RegisterSyncManager(/*ue::sys::core::I_SyncManager **/) = 0;
            virtual void UnregisterSyncManager(/*ue::sys::core::I_SyncManager **/) = 0;
            virtual void Synchronize(void)                                                                                                                          = 0;
            virtual void GetSynchronizationTiming(/*ue::sys::core::I_Core::S_SyncTiming &*/) = 0;
            virtual void RegisterEndFrameCallback(/*ue::sys::core::I_Core::I_EndFrameCallback &*/) = 0;
            virtual void UnRegisterEndFrameCallback(/*ue::sys::core::I_Core::I_EndFrameCallback &*/) = 0;
            virtual void RegisterSyncCallback(/*ue::I_Callback<void,ue::NullType> &*/) = 0;
            virtual void UnRegisterSyncCallback(/*ue::I_Callback<void,ue::NullType> &*/) = 0;
            virtual void *CreateRenderDestination(/*ue::sys::core::S_DestinationCreateParams* const&*/) = 0;
            virtual void DestroyRenderDestination(/*ue::C_Ptr<ue::sys::core::C_RenderDestination>*/) = 0;
            virtual void *GetRenderDestination(unsigned int)                                                                                                        = 0;
            virtual void *GetRenderDestinations(void)                                                                                                               = 0;
            virtual void BeginRenderFullscreenVideoMode(void)                                                                                                       = 0;
            virtual void EndRenderFullscreenVideoMode(void)                                                                                                         = 0;
            virtual bool IsFullScreenVideoModeActive(void)                                                                                                          = 0;
            virtual void BeginLoadingScreenMode(/*ue::sys::render::S_LoadingScreenDesc const&*/) = 0;
            virtual void EndLoadingScreenMode(void)                                                                                                                 = 0;
            virtual unsigned int GetNumLoadingScreenImagesRendered(void)                                                                                            = 0;
            virtual void SetRenderPresentationMode(bool, bool)                                                                                                      = 0;
            virtual void GetRenderPresentationMode(bool &)                                                                                                          = 0;
            virtual void RegisterExtendedModule(/*ue::sys::core::I_ExtendedModule * void**/) = 0;
            virtual void UnRegisterExtendedModule(/*ue::sys::core::I_ExtendedModule **/) = 0;
            virtual void RegisterRenderExtendedModule(/*ue::C_OwningPtr<ue::sys::render::I_ExtendedModule,ue::S_NoAllocatorPolicy,ue::S_NoWeakPtrPolicy> &*/) = 0;
            virtual void UnRegisterRenderExtendedModule(/*ue::C_Ptr<ue::sys::render::I_ExtendedModule>*/) = 0;
            virtual void RegisterShaderParamsProvider(/*ue::C_Ptr<ue::sys::render::I_ShaderParamsProvider>*/) = 0;
            virtual void UnRegisterShaderParamsProvider(/*ue::C_OwningPtr<ue::sys::render::I_ShaderParamsProvider,ue::S_NoAllocatorPolicy,ue::S_NoWeakPtrPolicy>*/) = 0;
            virtual void RegisterCoreCallbacks(/*ue::sys::core::I_CoreCallbacks **/) = 0;
            virtual void UnRegisterCoreCallbacks(/*ue::sys::core::I_CoreCallbacks **/) = 0;
            virtual void SaveShaderCachesOnNextSync(bool)                                                                                                           = 0;
            virtual void LoadShaderCachesOnNextSync(void)                                                                                                           = 0;
            virtual void ReloadShaderSourcesChanges(void)                                                                                                           = 0;
            virtual void ReloadShaderSourceFile(char const *, bool)                                                                                                 = 0;
            virtual unsigned int GetMainThreadId(void)                                                                                                              = 0;
            virtual void *GetSystemGraphicsDescriptor(void)                                                                                                         = 0;
            virtual void *GetSystemVideoModeDescriptor(void)                                                                                                        = 0;
            virtual void SetSystemVideoModeDescriptor(/*ue::sys::render::device::S_VideoModeDesc const&*/) = 0;
            virtual void Tick(float)                                                                                                                                = 0;
            virtual void RegisterForTick(/*uestd::function<bool ()(float)> &*/) = 0;
            virtual void UnregisterFromTick(unsigned long long)                                                                                                     = 0;
            virtual void ProcessRegisteredTickFunctions(float)                                                                                                      = 0;
            virtual void *GetImageFormatConvertor(void)                                                                                                             = 0;
            virtual float GetTime(void)                                                                                                                             = 0;
            virtual void *GetRenderConfig(void)                                                                                                                     = 0;
            virtual void OnRenderConfigChanged(void)                                                                                                                = 0;
            virtual void SetFPSLimit(float)                                                                                                                         = 0;
            virtual void AsyncDeleteMeshData(/*ue::C_CntPtr<ue::spatial::I_CollisionMeshSubdiv>,ue::C_CntPtr<ue::spatial::I_BPASubdiv>*/) = 0;
            virtual void *GetSceneManagerPtr(void)                                                                                                                  = 0;
            virtual void GetNeededLod(/*ue::sys::core::C_SceneObject const**/) = 0;
            virtual void *GetSyncTarget(void)                                                                                                                       = 0;
            virtual void *GetSyncManager(void)                                                                                                                      = 0;
            virtual void MarkObjectForSync(unsigned int)                                                                                                            = 0;
            virtual unsigned int GetSyncFlags(void)                                                                                                                 = 0;
            virtual void AddSyncFlags(unsigned int)                                                                                                                 = 0;

            char pad0[0xD8];
            void *m_pFrameClassFactory;                   // 00D8 - 00E0
            void *m_pVertexFormatManager;                 // 00E0 - 00E8
            void *m_pMemFileManager;                      // 00E8 - 00F0
            void *m_pXmlManager;                          // 00F0 - 00F8
            char pad1[0xE8];                              // 00F8 - 01E0
            ue::sys::render::C_RenderCore *m_pRenderCore; // 01E0 - 01E8
        };
    } // namespace ue::sys::core
} // namespace SDK