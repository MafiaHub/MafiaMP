#pragma once

#include <utils/states/state.h>

namespace MafiaMP::Core::States {
    class SessionDisconnectionState: public Framework::Utils::States::IState {
      public:
        SessionDisconnectionState();
        ~SessionDisconnectionState();

        virtual const char *GetName() const override;
        virtual int32_t GetId() const override;

        virtual bool OnEnter() override;
        virtual bool OnExit() override;

        virtual bool OnUpdate() override;
    };
} // namespace MafiaMP::Core::States
