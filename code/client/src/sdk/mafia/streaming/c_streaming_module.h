#pragma once

#include "../../ue/c_ptr.h"
#include "../../ue/c_string.h"
#include "../../ue/sys/math/c_vector.h"

namespace SDK {
    namespace mafia::streaming {
        enum class E_StreamingPosSource { PLAYER, CAMERA };

        typedef unsigned int E_StreamingAreaType;
        typedef void *I_WorldStateProvider;
        typedef unsigned int E_GraphicQuality;
        class S_StreamingAreaSDS;
        class S_StreamingArea;

        class C_StreamingModule {
          public:
            virtual ~C_StreamingModule()        = 0;
            virtual void *GetClassTypeInfo()    = 0;
            virtual void *GetTypeInfo()         = 0;
            virtual char *GetTickedModuleName() = 0;
            virtual void *StaticRegister()      = 0;
            // virtual void* GetTimeBudget(void) = 0;
            virtual void SetStreamingPosSource(E_StreamingPosSource)                                                                    = 0;
            virtual void *RequestStreamingLocationLoadedTest(unsigned int, bool)                                                        = 0;
            virtual void *RequestUnloadAll(unsigned int)                                                                                = 0;
            virtual void *RequestLoadAreasInRadius(SDK::ue::sys::math::C_Vector const &pos, float radius, E_StreamingAreaType areaType) = 0;
            virtual void *RequestLoadAreasInRadiusMask(SDK::ue::sys::math::C_Vector const &, float, unsigned int)                       = 0;
            virtual void *GetLoadRequestByUserData(unsigned long long)                                                                  = 0;
            virtual void *QueryAreas(float)                                                                                             = 0;
            virtual void *SetWorldStateProvider(SDK::ue::C_Ptr<I_WorldStateProvider>)                                                   = 0;
            virtual void *SetIncrementalStreaming(bool)                                                                                 = 0;
            virtual void *SetGraphicsSetting(E_GraphicQuality)                                                                          = 0;
            virtual void *GetAllowedSdsTypes(void)                                                                                      = 0;
            virtual void *GetSdsSize(SDK::ue::C_String const &)                                                                         = 0;
            virtual bool FitsInQuota(E_StreamingAreaType, unsigned long)                                                                = 0;
            virtual void *GetQuotaLimit(void)                                                                                           = 0;
            virtual void *GetQuotaLimitTextures(void)                                                                                   = 0;
            virtual void *GetQuotaUsed(void)                                                                                            = 0;
            virtual void *GetQuotaUsedTextures(void)                                                                                    = 0;
            virtual void *GetSdsState(S_StreamingAreaSDS const *)                                                                       = 0;
            virtual bool IsSdsLoaded(S_StreamingAreaSDS const *)                                                                        = 0;
            virtual void *OnSdsLoadStart(S_StreamingAreaSDS *)                                                                          = 0;
            virtual void *OnSdsLoadEnd(S_StreamingAreaSDS *)                                                                            = 0;
            virtual void *OnSdsUnloadStart(S_StreamingAreaSDS *)                                                                        = 0;
            virtual void *OnSdsUnloadEnd(S_StreamingAreaSDS *)                                                                          = 0;
            virtual void *OnNoQuotaSpaceFor(S_StreamingAreaSDS *, bool)                                                                 = 0;
            virtual void *OnSdsFail(S_StreamingAreaSDS *)                                                                               = 0;
            virtual void *OnTranslocation(S_StreamingArea *, int, int)                                                                  = 0;
        };
    } // namespace mafia::streaming
} // namespace SDK
