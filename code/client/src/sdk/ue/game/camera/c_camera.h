#pragma once

#include <cstdint>
#include "../../sys/math/c_vector.h"
#include "../../sys/math/c_matrix.h"
#include "../../sys/core/c_scene_object.h"

namespace SDK {
        namespace ue::game::camera {
            typedef unsigned int E_CameraBlendMode;
            typedef void *I_CameraDataProvider;

            enum class E_CameraCommandID {
                CAMCOMMAND_SETPOS      = 2,
                CAMCOMMAND_LOCKCONTROL = 1282368363,
                CAMCOMMAND_GAMEOVER    = 1381191748,
                CAMCOMMAND_SHAKE       = 1399349587,
            };

            enum class E_GameCameraModeID {
                E_GCM_NONE                        = 0,
                E_GCM_MAFIA_LADDER                = 0,
                E_GCM_DEBUG                       = 1,
                E_GCM_DEBUG_ALTERNATE             = 2,
                E_GCM_CUTSCENE                    = 3,
                E_GCM_DIALOG                      = 4,
                E_GCM_ANIMATED                    = 5,
                E_GCM_AIDEBUG                     = 6,
                E_GCM_MAFIA_ENUMBEGIN             = 7,
                E_GCM_MAFIA_LIMITEDCONTROL        = 8,
                E_GCM_MAFIA_PLAYER                = 9,
                E_GCM_MAFIA_BONE                  = 10,
                E_GCM_MAFIA_COVER                 = 11,
                E_GCM_MAFIA_AIM                   = 12,
                E_GCM_MAFIA_RAIL                  = 13,
                E_GCM_MAFIA_CAR_STATIC_BUMPER     = 14,
                E_GCM_MAFIA_CAR_STATIC_WHEEL      = 15,
                E_GCM_MAFIA_CAR_STATIC_HOOD       = 16,
                E_GCM_MAFIA_CAR_STATIC_LOOKBACK   = 17,
                E_GCM_MAFIA_CAR_DRIVERSHOOTFOLLOW = 18,
                E_GCM_MAFIA_CAR_PASSENGERSHOOT    = 19,
                E_GCM_MAFIA_CAR_CARGOSPACESHOOT   = 20,
                E_GCM_MAFIA_CAR_CARGOSPACELIMITED = 21,
                E_GCM_MAFIA_CAR_FOLLOW            = 22,
                E_GCM_MAFIA_TRUCK_FOLLOW          = 23,
                E_GCM_MAFIA_MOTORCYCLE_FOLLOW     = 24,
                E_GCM_MAFIA_MOTORCYCLE_RIDER      = 25,
                E_GCM_MAFIA_MOTORCYCLE_LOOKBACK   = 26,
                E_GCM_MAFIA_TRAIN_INTERNAL_FOLLOW = 27,
                E_GCM_MAFIA_TRAIN_EXTERNAL_DOOR   = 28,
                E_GCM_MAFIA_MELEE                 = 29,
                E_GCM_MAFIA_CAR                   = 30,
                E_GCM_MAFIA_GARAGE                = 31,
                E_GCM_SHOP_MENU                   = 32,
                E_GCM_SCRIPT                      = 33,
                E_GCM_SCRIPTOVER                  = 34,
                E_GCM_SCRIPTFRAME                 = 35,
                E_GCM_STATIC                      = 36,
                E_GCM_FPV                         = 37,
                E_GCM_DEATH                       = 38,
                E_GCM_OPEN_AUTOMATE               = 39,
                E_GCM_MAFIA_CAR_INITIAL           = 40,
                E_GCM_MAFIA_BOAT                  = 41,
                E_GCM_MAFIA_BOAT_FOLLOW           = 42,
                E_GCM_MAFIA_BOAT_BOW              = 43,
                E_GCM_MAFIA_BOAT_GUNWALE          = 44,
                E_GCM_MAFIA_BOAT_ROOF             = 45,
                E_GCM_MAFIA_BOAT_LOOKBACK         = 46,
                E_GCM_MAFIA_BOAT_DRIVERSHOOT      = 47,
                E_GCM_MAFIA_BOAT_PASSENGERSHOOT   = 48,
                E_GCM_VEHICLE_DEBUG               = 49,
                E_GCM_BOAT_DEBUG                  = 50,
                E_GCM_TRAP_CAMERA                 = 51,
                E_GCM_LADDER                      = 52,
                E_GCM_SWIM                        = 53,
                E_GCM_LAST                        = 54,
            };

            enum class E_GameCameraLayer : unsigned int { LAYER_ACTOR = 1, LAYER_UNKNOWN = 2 };

            enum class E_CameraCommandSetPosMode : unsigned int { CAMCOMMAND_SETPOS_MODE_DIRECT = 0, CAMCOMMAND_SETPOS_MODE_INTERPOLATE = 1 };

            struct S_CameraCommandSetPosData {
                ue::sys::math::C_Vector start;  // 0000 - 000C
                ue::sys::math::C_Vector end;    // 000C - 0018
                uint32_t flags;                 // 0018 - 001C
                uint32_t unk3;                  // 001C - 0020
                int32_t unk4;                   // 0020 - 0024
                uintptr_t *unk5;                // 0024 - 0028
                uintptr_t *unk6;                // 0028 - 002C
                E_CameraCommandSetPosMode mode; // 002C - 0030
            };

            struct S_CameraCommandShakeData {
                float speed;    // 0000 - 0004
                float strength; // 0004 - 0008
                float duration; // 0008 - 000C
            };

            class C_Camera {
              public:
                virtual void GetPosDir(sys::math::C_Vector &pos, sys::math::C_Vector &dir)                                                                              = 0;
                virtual const sys::math::C_Vector GetPos()                                                                                                              = 0;
                virtual void *GetTarget()                                                                                                                               = 0;
                virtual const sys::math::C_Vector GetTargetPos()                                                                                                        = 0;
                virtual const sys::math::C_Vector GetSafeTargetPos()                                                                                                    = 0;
                virtual const sys::math::C_Vector GetHeadPos()                                                                                                          = 0;
                virtual const sys::math::C_Vector GetUp()                                                                                                               = 0;
                virtual const sys::math::C_Vector GetRight()                                                                                                            = 0;
                virtual const sys::math::C_Vector GetDir()                                                                                                              = 0;
                virtual const sys::math::C_Vector GetDirNoSlope()                                                                                                       = 0;
                virtual const float GetDistance()                                                                                                                       = 0;
                virtual const float GetMaxDistance()                                                                                                                    = 0;
                virtual void GetScreenPos(ue::sys::math::C_Vector2 &screenPos, ue::sys::math::C_Vector const &worldPos, bool &isVisible, float *, float *, bool) const  = 0;
                virtual void Enable(bool enable)                                                                                                                        = 0;
                virtual const bool Enabled() const                                                                                                                      = 0;
                virtual const sys::math::C_Matrix &GetViewMatrix()                                                                                                      = 0;
                virtual const sys::math::C_Matrix &GetProjectionMatrix()                                                                                                = 0;
                virtual const sys::math::C_Matrix &GetViewProjMatrix()                                                                                                  = 0;
                virtual void SetAspectRatio(float ratio)                                                                                                                = 0;
                virtual float GetAspectRatio()                                                                                                                          = 0;
                virtual float GetFrustum()                                                                                                                              = 0;
                virtual float GetFOV()                                                                                                                                  = 0;
                virtual float GetTopModeBaseFOV()                                                                                                                       = 0;
                virtual void SetTopModeBaseFOV(float)                                                                                                                   = 0;
                virtual void GetRange(float &, float &)                                                                                                                 = 0;
                virtual void SetRange(float, float)                                                                                                                     = 0;
                virtual void SkipInterpolation()                                                                                                                        = 0;
                virtual void SendCommand(E_CameraCommandID, void *, void *)                                                                                             = 0;
                virtual void BroadcastCommand(E_CameraCommandID, void *, void *)                                                                                        = 0;
                virtual void EnableBlendMode(ue::game::camera::E_CameraBlendMode, bool)                                                                                 = 0;
                virtual void PrecalculateDirection(float)                                                                                                               = 0;
                virtual float GetLODMeter()                                                                                                                             = 0;
                virtual void SetFStop(float)                                                                                                                            = 0;
                virtual void SetFocusDistance(float)                                                                                                                    = 0;
                virtual void SetFocalLength(float)                                                                                                                      = 0;
                virtual void SetShutterSpeed(float)                                                                                                                     = 0;
                virtual void *GetAimPoint()                                                                                                                             = 0;
                virtual void SetReferenceCameraBone(char const *)                                                                                                       = 0;
                virtual char *GetReferenceCameraBone()                                                                                                                  = 0;
                virtual void SetAnimStateHandle(void *ue__game__anim__S_WAnimStateHandle)                                                                               = 0;
                virtual void *GetAnimStateHandle()                                                                                                                      = 0;
                virtual void SetSavedDirection(ue::sys::math::C_Vector)                                                                                                 = 0;
                virtual ue::sys::math::C_Vector GetSavedDirection()                                                                                                     = 0;
                virtual void SetSavedObjectMatrix(ue::sys::math::C_Matrix)                                                                                              = 0;
                virtual const sys::math::C_Matrix &GetSavedObjectMatrix(void)                                                                                           = 0;
                virtual bool GetDebugMode(void)                                                                                                                         = 0;
                virtual void SetDebugMode(bool)                                                                                                                         = 0;
                virtual bool GetDebugModeObject(void)                                                                                                                   = 0;
                virtual void SetDebugModeObject(void *C_CameraMode)                                                                                                     = 0;
                virtual void SetAcquireGamePadInputDebugMode(bool)                                                                                                      = 0;
                virtual bool GetAcquireGamePadInputDebugMode(void)                                                                                                      = 0;
                virtual void SetUseExtraGamepadInDebugMode(bool)                                                                                                        = 0;
                virtual bool GetUseExtraGamepadInDebugMode(void)                                                                                                        = 0;
                virtual bool GetPreserveAimController(void)                                                                                                             = 0;
                virtual void SetPreserveAimController(/*ue::C_OwningPtr<ue::game::camera::I_PreserveAimController, ue::S_NoAllocatorPolicy, ue::S_NoWeakPtrPolicy>&&*/) = 0;
                virtual void *GetAimAssistController()                                                                                                                  = 0;
                virtual void SetAimAssistController(/*ue::C_OwningPtr<ue::game::camera::I_AimAssistController, ue::S_NoAllocatorPolicy, ue::S_NoWeakPtrPolicy>&&*/) = 0;
                virtual void PostModeChangeRequest(E_GameCameraModeID, I_CameraDataProvider *, bool, bool)                                                              = 0;
                virtual void PostModeChangeRequestWithCommand(E_GameCameraModeID, I_CameraDataProvider *, unsigned int, void *, void *, bool, bool)                     = 0;
                virtual void PostPopRequest(bool, E_GameCameraLayer)                                                                                                    = 0;
                virtual E_GameCameraModeID ModeGetActiveTypeTop()                                                                                                       = 0;
                virtual void *ModeGetActiveObjectTop(void)                                                                                                              = 0;
                virtual bool IsModePresent(E_GameCameraModeID)                                                                                                          = 0;
                virtual void Reset()                                                                                                                                    = 0;
                virtual void *ModeChangeImmediate(E_GameCameraModeID, I_CameraDataProvider *, bool, bool, bool)                                                         = 0;
                virtual void ModePopImmediate(bool, E_GameCameraLayer, bool)                                                                                            = 0;
                virtual void ForceUpdateAllPhases(void)                                                                                                                 = 0;
                virtual bool IsForceUpdating()                                                                                                                          = 0;
                virtual void SetCameraObject(/*ue::C_Ptr<ue::sys::core::C_SceneObject> const&*/ void *);
                virtual void *GetCameraObject(void)                                                                             = 0;
                virtual bool ModeIsPushed(E_GameCameraLayer)                                                                    = 0;
                virtual bool ClearRequestsWithProvider(I_CameraDataProvider *)                                                  = 0;
                virtual void ClearRequests(void)                                                                                = 0;
                virtual bool HasRequests(void)                                                                                  = 0;
                virtual void *GetModeStack(void)                                                                                = 0;
                virtual void *GetRenderDestination()                                                                            = 0;
                virtual void *GetRenderContext()                                                                                = 0;
                virtual void *GetScene()                                                                                        = 0;
                virtual void MapScreenTarget(float &x, float &y)                                                                = 0;
                virtual void GetCurrentVectors(ue::sys::math::C_Vector &, ue::sys::math::C_Vector &, ue::sys::math::C_Vector &) = 0;
                virtual float GetShakeShift(void)                                                                               = 0;
                virtual float GetShakeOverride(void)                                                                            = 0;
                virtual bool GetShakeOverrideEnabled(void)                                                                      = 0;
                virtual void SetForcedVehicleFollowCamera(bool)                                                                 = 0;
                virtual bool GetForcedVehicleFollowCamera(void)                                                                 = 0;
                virtual void SetWaterLevel(float)                                                                               = 0;
                virtual float GetWaterLevel(void)                                                                               = 0;
                virtual sys::math::C_Vector GetBaseTargetPos(void)                                                              = 0;
                virtual sys::math::C_Vector GetBaseDir(void)                                                                    = 0;

                ue::sys::core::C_SceneObject *GetSceneObject() {
                    return *reinterpret_cast<ue::sys::core::C_SceneObject **>(this + 0x41);
                }
            };
        }; // namespace game::camera
}
