#pragma once

#include <cstdint>
#include "../../sys/math/c_vector.h"
#include "../../sys/math/c_matrix.h"
#include "../../sys/core/c_scene_object.h"
#include "sdk/ue/sys/core/c_camera_component.h"
#include "sdk/ue/sys/render/c_render_context.h"

#include "e_game_camera_mode_id.h"

namespace SDK {
        namespace ue::game::camera {
            typedef unsigned int E_CameraBlendMode;
            typedef void *I_CameraDataProvider;

            enum class E_CameraCommandID {
                CCM_POINT_AT_VEC = 5,
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

            // HINT: this + 0x37 is probably C_CameraMode
            class C_Camera {
              public:
                char pad0[0x10];                                                    // 0000 - 0010
                bool m_bEnabled;                                                    // 0010 - 0011
                char pad1[0x33];                                                    // 0011 - 0044
                ue::sys::render::C_RenderContext *m_pRenderContext;                 // 0044 - 004C
                char pad2[0x1BC];                                                   // 004C - 0208
                char *m_sCameraObject;                                              // 0208 - 0210
                ue::sys::core::C_CameraComponent *m_pCameraComponent;               // 0210 - 0218

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
                virtual const bool IsEnabled() const                                                                                                                      = 0;
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
                virtual ue::sys::render::C_RenderContext *GetRenderContext()                                                    = 0;
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
