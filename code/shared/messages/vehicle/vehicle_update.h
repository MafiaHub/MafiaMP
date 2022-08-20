#pragma once

#include "../messages.h"
#include <networking/messages/game_sync/message.h>

#include <glm/glm.hpp>

namespace MafiaMP::Shared::Messages::Vehicle {
    class VehicleUpdate final: public Framework::Networking::Messages::GameSyncMessage {
      private:
        glm::vec3 _velocity{};
        glm::vec3 _angularVelocity{};
        int _gear{};
        bool _horn{};
        float _power{};
        float _brake{};
        float _handbrake{};
        float _steer{};

      public:
        uint8_t GetMessageID() const override {
            return MOD_VEHICLE_UPDATE;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _velocity);
            bs->Serialize(write, _angularVelocity);
            bs->Serialize(write, _gear);
            bs->Serialize(write, _horn);
            bs->Serialize(write, _power);
            bs->Serialize(write, _brake);
            bs->Serialize(write, _handbrake);
            bs->Serialize(write, _steer);
        }

        bool Valid() const override {
            // todo
            return true;
        }

        glm::vec3 GetVelocity() const {
            return _velocity;
        }

        glm::vec3 GetAngularVelocity() const {
            return _angularVelocity;
        }

        int GetGear() const {
            return _gear;
        }

        bool GetHorn() const {
            return _horn;
        }

        float GetPower() const {
            return _power;
        }

        float GetBrake() const {
            return _brake;
        }

        float GetHandbrake() const {
            return _handbrake;
        }

        float GetSteer() const {
            return _steer;
        }

        void SetVelocity(const glm::vec3 &velocity) {
            _velocity = velocity;
        }

        void SetAngularVelocity(const glm::vec3 &angularVelocity) {
            _angularVelocity = angularVelocity;
        }

        void SetGear(int gear) {
            _gear = gear;
        }

        void SetHorn(bool horn) {
            _horn = horn;
        }

        void SetPower(float power) {
            _power = power;
        }

        void SetBrake(float brake) {
            _brake = brake;
        }

        void SetHandbrake(float handbrake) {
            _handbrake = handbrake;
        }

        void SetSteer(float steer) {
            _steer = steer;
        }
    };
} // namespace MafiaMP::Shared::Messages::Vehicle
