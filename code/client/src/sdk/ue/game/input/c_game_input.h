#pragma once

#include <cstdint>

namespace SDK {
    namespace ue::game::input {
        class C_InputSet;

        class C_GameInput {
          public:
            virtual ~C_GameInput() = default;

            static C_GameInput *GetInstance();

            C_InputSet *CreateInputSet();
            void DestroyInputSet(C_InputSet *pInputSet);
            void Update();
            void FindDevice(void *outDevices, void *filter);
        };

        void Initialize(void *pInit);
        bool IsInitialized();
        void Deinitialize();
    } // namespace ue::game::input
} // namespace SDK
