#pragma once

#include "../../sdk/entities/c_human_2.h"
#include "../../sdk/ue/game/humainai/c_character_controller.h"
#include "../../sdk/ue/game/humainai/c_character_state_handler.h"

extern thread_local bool CreateNetCharacterController;

namespace MafiaMP::Game::Overrides {
    class CharacterController : public SDK::ue::game::humanai::C_CharacterController {
      private:
        static uintptr_t _vfTables[2];

        SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type _desiredHandlerType = SDK::ue::game::humanai::C_CharacterStateHandler::E_SHT_NONE;
        SDK::E_HumanMoveMode _humanMoveMode                                                         = SDK::E_HumanMoveMode::E_HMM_NONE;
        bool _stalkMoveOverride                                                                   = false;
        bool _isSprintingOverride                                                                 = false;
        float _sprintMoveSpeed                                                            = 0.0f;

        CharacterController();

      public:
        typedef void *(__fastcall *OrigConstructor_t)(SDK::ue::game::humanai::C_CharacterController *, SDK::C_Human2 *);

        virtual ~CharacterController();
        void operator delete(void *);

        static CharacterController *Create(OrigConstructor_t, SDK::C_Human2 *);
        static bool IsInstanceOfClass(void *);

        uint64_t GetState(uintptr_t) override;
        uint64_t GetMoveMode() override;

        void OnEvent(SDK::C_Entity *, int) override;

        void ActivateVehicleStateHandler(SDK::ue::game::humanai::C_CharacterStateHandler *, bool unk);
        void SwitchHandler(SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type desiredHandlerType);
        void OnUpdateLocomotionHandlers(float dt);

        void SetDesiredHandlerType(SDK::ue::game::humanai::C_CharacterStateHandler::E_State_Handler_Type desiredHandlerType) {
            _desiredHandlerType = desiredHandlerType;
        }

        void SetStalkMoveOverride(bool shouldStalk) {
            _stalkMoveOverride = shouldStalk;
        }
        void SetMoveStateOverride(SDK::E_HumanMoveMode hmm, bool isSprinting, float sprintMoveSpeed) {
            _humanMoveMode            = hmm;
            _isSprintingOverride     = isSprinting;
            _sprintMoveSpeed          = sprintMoveSpeed;
        }
    };
}
