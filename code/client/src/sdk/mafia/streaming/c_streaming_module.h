#pragma once

namespace SDK {
    namespace mafia::streaming {
        enum class E_StreamingPosSource { PLAYER, CAMERA };

        class C_StreamingModule {
          public:
            virtual ~C_StreamingModule()        = 0;
            virtual void *GetClassTypeInfo()    = 0;
            virtual void *GetTypeInfo()         = 0;
            virtual char *GetTickedModuleName() = 0;
            virtual void *StaticRegister()      = 0;
            virtual void *GetTimeBudget(void)   = 0;
            void SetStreamingPosSource(E_StreamingPosSource source);
        };
    } // namespace mafia::streaming
}
