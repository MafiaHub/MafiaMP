#pragma once

#include "../entities/c_car.h"
#include "../entities/c_actor.h"
#include "../ue/sys/math/c_vector.h"

namespace SDK {
    class I_Human2WrapperBase {
      public:
        C_Car *GetActor() const {
            return m_pUsedCar;
        }

      public:
        char _pad0[0x18];                 // 0x00 - 0x18
        C_Car *m_pUsedCar;                // 0x18 - 0x20
    };

    class C_Human2CarWrapper : public I_Human2WrapperBase {
      public:
        struct S_SeatInfo {
            ue::sys::math::C_Vector m_vPosition;  // 0x00 - 0x0C
            int32_t m_nDoorState;                 // 0x0C - 0x10
            bool m_bIsDriver;                     // 0x10 - 0x11
            char _pad0[0x3];                      // 0x11 - 0x14
        };

        enum E_DoorState : int32_t {
            E_DOOR_OPEN    = 0,
            E_DOOR_OPENING = 1,
            E_DOOR_CLOSED  = 2
        };

      public:
        // Seat management
        unsigned int GetSeatID(C_Actor *pActor) const;
        C_Actor *GetHumanOnSeat(unsigned int seatIdx) const;
        bool IsFreeSeat(unsigned int seatIdx) const;
        bool IsSeatReserved(C_Actor *pActor, unsigned int seatIdx);
        bool ReserveSeat(unsigned int seatIdx, C_Actor *pActor, bool createTask);

        unsigned int GetDriverSeatID() const {
            return 0;
        }

        bool IsDriverSeat(unsigned int seatIdx) const {
            return seatIdx == 0;
        }

        // Enter/exit vehicle
        unsigned int GetIn(C_Actor *pActor, unsigned int seatIdx, S_SeatInfo &seatInfo);
        void GetOut(C_Actor *pActor);
        bool StartChangeSeat(C_Actor *pActor);
        bool EndChangeSeat(C_Actor *pActor);

        // Driver checks
        bool IsDriver(C_Actor *pActor) const;
        bool PlayerOnPassSeatAndDriverFree(unsigned int seatIdx) const;
        bool PlayerOnPassSeatAndDriverFreeingUp(unsigned int seatIdx) const;

        // Door management
        bool DoorExists(unsigned int seatIdx) const;
        int GetDoorID(unsigned int seatIdx) const;
        E_DoorState GetDoorState(unsigned int seatIdx, bool checkOpening) const;
        float GetDoorAngle(unsigned int doorIdx) const;
        bool IsDoorFree(unsigned int doorIdx) const;
        void SetDoorFree(int doorIdx, bool free);
        bool OpenDoor(int doorIdx, bool force, bool instant);
        bool BreakWindow(unsigned int seatIdx);

        // Vehicle state
        bool IsEngineOn() const;
        bool IsAutomatic() const;
        int GetGear() const;
        bool IsTaxi() const;
        bool HasSpeedLimit() const;
        float GetSpeedLimit() const;

        // Vehicle controls
        void SetControls(float steering, float throttle, float brake, int gear, bool handbrake, bool horn, bool lights, bool leftIndicator, bool rightIndicator, int sirenMode, bool clutch);
    };

    static_assert(sizeof(I_Human2WrapperBase) == 0x20, "I_Human2WrapperBase size mismatch");
} // namespace SDK
