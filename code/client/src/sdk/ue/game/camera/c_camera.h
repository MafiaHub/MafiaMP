#pragma once

#include <cstdint>
#include "../../sys/math/c_vector.h"
#include "../../sys/math/c_matrix.h"
#include "../../sys/core/c_scene_object.h"
#include "sdk/ue/sys/core/c_camera_component.h"
#include "sdk/ue/sys/render/c_render_context.h"

#include "c_camera_render_data.h"
#include "e_game_camera_mode_id.h"
#include "s_forced_look.h"

namespace SDK {
    namespace ue::game::camera {
        typedef unsigned int E_CameraBlendMode;
        typedef void *I_CameraDataProvider;

        enum class E_CameraCommandID {
            CCM_UNK_0              = 0,
            CCM_UNK_1              = 1,
            CCM_UNK_2              = 2,
            CCM_UNK_3              = 3,
            CCM_UNK_4              = 4,
            CCM_POINT_AT_VEC       = 5,
            CCM_LOOK_AT_STOP       = 6,
            CCM_UNK_9              = 9,
            CCM_UNK_10             = 10,
            CCM_UNK_11             = 11,
            CCM_UNK_12             = 12,
            CCM_UNK_13             = 13,
            CCM_SET_SHAKE_PARAMS   = 14,
            CCM_SET_SHAKE_ENABLED  = 15,
            CCM_UNK_16             = 16,
            CCM_FORCED_FOV         = 17,
            CCM_UNK_18             = 18,
        };

        enum class E_GameCameraLayer : unsigned int { LAYER_ACTOR = 1, LAYER_UNKNOWN = 2 };

        // Forward declarations
        class C_CameraMode;

        /**
         * C_Camera - Base camera class
         * Size: 0x328 (808 bytes)
         *
         * Note: vtable at 0x00 is implicit from virtual methods
         */
        class C_Camera {
          public:
            void *m_pScriptEntityVtable;                                    // 0008 - 0010 (I_ScriptEntity interface)
            bool m_bEnabled;                                                // 0010 - 0011
            char pad0[0x02];                                                // 0011 - 0013
            char pad1[0x61];                                                // 0013 - 0074
            float m_fUnk74;                                                 // 0074 - 0078 (-4.0f)
            uint64_t m_iUnk78;                                              // 0078 - 0080
            uint32_t m_iUnk80;                                              // 0080 - 0084
            sys::math::C_Vector m_vUnk84;                                   // 0084 - 0090
            C_CameraRenderData m_RenderData;                                // 0090 - 0190
            void *m_pRenderDataPtr;                                         // 0190 - 0198 (pointer to m_RenderData)
            void *m_pPreserveAimController;                                 // 0198 - 01A0
            void *m_pAimAssistController;                                   // 01A0 - 01A8
            uint16_t m_iCameraID;                                           // 01A8 - 01AA
            char pad2[0x06];                                                // 01AA - 01B0
            void *m_pModeStackStart;                                        // 01B0 - 01B8 (std::vector<C_CameraMode*>)
            void *m_pModeStackEnd;                                          // 01B8 - 01C0
            void *m_pModeStackCapacity;                                     // 01C0 - 01C8
            char pad3[0x01];                                                // 01C8 - 01C9
            uint16_t m_iUnk1C9;                                             // 01C9 - 01CB
            bool m_bUnk1CB;                                                 // 01CB - 01CC
            char pad4[0x2C];                                                // 01CC - 01F8
            void *m_pRefCountPtr1;                                          // 01F8 - 0200
            void *m_pSceneObject;                                           // 0200 - 0208
            char *m_sCameraObject;                                          // 0208 - 0210 (camera object name)
            void *m_pUnk210;                                                // 0210 - 0218
            void *m_pScene;                                                 // 0218 - 0220
            void *m_pRenderContext;                                         // 0220 - 0228
            void *m_pRenderDestination;                                     // 0228 - 0230
            bool m_bUnk230;                                                 // 0230 - 0231
            char pad5[0x0B];                                                // 0231 - 023C
            sys::math::C_Vector m_vUnk23C;                                  // 023C - 0248
            void *m_pUnk248;                                                // 0248 - 0250
            void *m_pUnk250;                                                // 0250 - 0258
            uint64_t m_iUnk258;                                             // 0258 - 0260
            uint32_t m_iUnk260;                                             // 0260 - 0264
            char pad6[0x04];                                                // 0264 - 0268
            char m_Unk268[0x10];                                            // 0268 - 0278 (oword)
            char m_Unk278[0x10];                                            // 0278 - 0288 (oword)
            char m_Unk288[0x10];                                            // 0288 - 0298 (oword)
            S_ForcedLook m_ForcedLook;                                      // 0298 - 02B0
            void *m_pUnk2B0;                                                // 02B0 - 02B8
            void *m_pUnk2B8;                                                // 02B8 - 02C0
            void *m_pUnk2C0;                                                // 02C0 - 02C8
            void *m_pRefCountPtr2;                                          // 02C8 - 02D0
            void *m_pUnk2D0;                                                // 02D0 - 02D8
            uint16_t m_iUnk2D8;                                             // 02D8 - 02DA
            char pad7[0x1E];                                                // 02DA - 02F8
            uint32_t m_iUnk2F8;                                             // 02F8 - 02FC
            char pad8[0x04];                                                // 02FC - 0300
            void *m_pRequestsStart;                                         // 0300 - 0308
            void *m_pRequestsEnd;                                           // 0308 - 0310
            void *m_pRequestsCapacity;                                      // 0310 - 0318
            uint32_t m_iUnk318;                                             // 0318 - 031C
            uint16_t m_iUnk31C;                                             // 031C - 031E
            char pad9[0x02];                                                // 031E - 0320
            bool m_bInitialized;                                            // 0320 - 0321
            char pad10[0x07];                                               // 0321 - 0328

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
            virtual const bool IsEnabled() const                                                                                                                    = 0;
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
            virtual void SetAimAssistController(/*ue::C_OwningPtr<ue::game::camera::I_AimAssistController, ue::S_NoAllocatorPolicy, ue::S_NoWeakPtrPolicy>&&*/)     = 0;
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
            virtual void SetCameraObject(/*ue::C_Ptr<ue::sys::core::C_SceneObject> const&*/ void *)                                                                 = 0;
            virtual void *GetCameraObject(void)                                                                                                                     = 0;
            virtual bool ModeIsPushed(E_GameCameraLayer)                                                                                                            = 0;
            virtual bool ClearRequestsWithProvider(I_CameraDataProvider *)                                                                                          = 0;
            virtual void ClearRequests(void)                                                                                                                        = 0;
            virtual bool HasRequests(void)                                                                                                                          = 0;
            virtual void *GetModeStack(void)                                                                                                                        = 0;
            virtual void *GetRenderDestination()                                                                                                                    = 0;
            virtual ue::sys::render::C_RenderContext *GetRenderContext()                                                                                            = 0;
            virtual void *GetScene()                                                                                                                                = 0;
            virtual void MapScreenTarget(float &x, float &y)                                                                                                        = 0;
            virtual void GetCurrentVectors(ue::sys::math::C_Vector &, ue::sys::math::C_Vector &, ue::sys::math::C_Vector &)                                         = 0;
            virtual float GetShakeShift(void)                                                                                                                       = 0;
            virtual float GetShakeOverride(void)                                                                                                                    = 0;
            virtual bool GetShakeOverrideEnabled(void)                                                                                                              = 0;
            virtual void SetForcedVehicleFollowCamera(bool)                                                                                                         = 0;
            virtual bool GetForcedVehicleFollowCamera(void)                                                                                                         = 0;
            virtual void SetWaterLevel(float)                                                                                                                       = 0;
            virtual float GetWaterLevel(void)                                                                                                                       = 0;
            virtual sys::math::C_Vector GetBaseTargetPos(void)                                                                                                      = 0;
            virtual sys::math::C_Vector GetBaseDir(void)                                                                                                            = 0;

            ue::sys::core::C_SceneObject *GetSceneObject() {
                return reinterpret_cast<ue::sys::core::C_SceneObject *>(m_pSceneObject);
            }
        };

        // Static size: 0x328 (808 bytes)
        static_assert(sizeof(C_Camera) == 0x328, "C_Camera size mismatch");
    } // namespace ue::game::camera
} // namespace SDK
