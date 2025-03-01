#pragma once

#include <utils/states/state.h>

namespace MafiaMP::Core::States {
    class MainMenuState: public Framework::Utils::States::IState {
      private:
        bool _shouldProceedConnection;
        bool _shouldProceedOfflineDebug;

      public:
        MainMenuState();
        ~MainMenuState() override;

        virtual const char *GetName() const override;
        virtual int32_t GetId() const override;

        virtual bool OnEnter(Framework::Utils::States::Machine *) override;
        virtual bool OnExit(Framework::Utils::States::Machine *) override;

        virtual bool OnUpdate(Framework::Utils::States::Machine *) override;
    };
} // namespace MafiaMP::Core::States
