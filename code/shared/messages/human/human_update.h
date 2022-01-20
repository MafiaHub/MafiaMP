#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

namespace MafiaMP::Shared::Messages::Human {
    class HumanUpdate final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        uint8_t _healthPercent;

        uint8_t _charStateHandlerType;
        uint8_t _moveMode;

        bool _isSprinting;
        bool _isStalking;

        float _sprintSpeed;

      public:
        uint8_t GetMessageID() const override {
            return MOD_HUMAN_UPDATE;
        }

        void FromParameters(flecs::entity_t serverID) {
            _serverID = serverID;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _serverID);
            bs->Serialize(write, _healthPercent);
            bs->Serialize(write, _charStateHandlerType);
            bs->Serialize(write, _moveMode);
            bs->Serialize(write, _isSprinting);
            bs->Serialize(write, _isStalking);
            bs->Serialize(write, _sprintSpeed);
        }

        bool Valid() override {
            return ValidServerID();
        }

        void SetHealthPercent(uint8_t percent) {
            _healthPercent = percent;
        }

        uint8_t GetHealthPercent() const {
            return _healthPercent;
        }

        void SetCharStateHandlerType(uint8_t type) {
            _charStateHandlerType = type;
        }

        uint8_t GetCharStateHandlerType() const {
            return _charStateHandlerType;
        }

        void SetMoveMode(uint8_t mode) {
            _moveMode = mode;
        }

        uint8_t GetMoveMode() const {
            return _moveMode;
        }

        void SetSprinting(bool sprinting) {
            _isSprinting = sprinting;
        }

        bool IsSprinting() const {
            return _isSprinting;
        }

        void SetStalking(bool stalking) {
            _isStalking = stalking;
        }

        bool IsStalking() const {
            return _isStalking;
        }

        void SetSprintSpeed(float speed) {
            _sprintSpeed = speed;
        }

        float GetSprintSpeed() const {
            return _sprintSpeed;
        }
    };
} // namespace MafiaMP::Shared::Messages::Human
