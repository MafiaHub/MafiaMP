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
         * Reversed from:
         * - Constructor: ue::game::camera::C_Camera::C_Camera (0x1409963d0)
         * - Reset: ue::game::camera::C_Camera::Reset (0x1409b2310)
         * - GetRight/GetUp/GetDir/GetPos accessors
         *
         * Allocation size confirmed from C_MafiaCameraModule::SystemInit (0x142de89c0)
         * operator_new(832) for C_GameCameraMafia, with base class ending at 0x328
         */
        class C_Camera {
          public:
            void *m_pScriptEntityVtable;                                    // 0x08 - 0x10 (I_ScriptEntity interface)

            // Section 0x10 - 0x18: Enabled flag and padding
            bool m_bEnabled;                                                // 0x10 - 0x11
            uint8_t m_bFlags11;                                             // 0x11 - 0x12 (WORD at 0x11, low byte)
            uint8_t m_bFlags12;                                             // 0x12 - 0x13 (WORD at 0x11, high byte)
            uint8_t m_pad13[0x05];                                          // 0x13 - 0x18

            // Section 0x18 - 0x48: View matrix (stores right/dir/up/pos vectors)
            sys::math::C_Matrix m_mViewMatrix;                              // 0x18 - 0x48

            // Section 0x48 - 0x70: Cached direction vectors (from Reset)
            sys::math::C_Vector m_vCachedRight;                             // 0x48 - 0x54 (init {0,0,0})
            sys::math::C_Vector m_vCachedUp;                                // 0x54 - 0x60 (init {0,1,0})
            sys::math::C_Vector m_vCachedDir;                               // 0x60 - 0x6C (init {0,0,1})
            float m_fUnk6C;                                                 // 0x6C - 0x70 (init 50.0f)

            // Section 0x70 - 0x90: More camera parameters
            uint32_t m_dwPad70;                                             // 0x70 - 0x74
            float m_fUnk74;                                                 // 0x74 - 0x78 (init -1.0f)
            float m_fFOVRadians;                                            // 0x78 - 0x7C (init 70.0f in radians = 1.22173)
            float m_fUnk7C;                                                 // 0x7C - 0x80
            uint32_t m_dwUnk80;                                             // 0x80 - 0x84
            sys::math::C_Vector m_vUnk84;                                   // 0x84 - 0x90

            // Section 0x90 - 0x190: Render data (256 bytes)
            C_CameraRenderData m_RenderData;                                // 0x90 - 0x190

            // Section 0x190 - 0x1B0: Pointers and camera ID
            void *m_pRenderDataPtr;                                         // 0x190 - 0x198
            void *m_pPreserveAimController;                                 // 0x198 - 0x1A0
            void *m_pAimAssistController;                                   // 0x1A0 - 0x1A8
            uint16_t m_iCameraID;                                           // 0x1A8 - 0x1AA
            uint8_t m_pad1AA[0x06];                                         // 0x1AA - 0x1B0

            // Section 0x1B0 - 0x1D4: Mode stack and flags
            void *m_pModeStackStart;                                        // 0x1B0 - 0x1B8
            void *m_pModeStackEnd;                                          // 0x1B8 - 0x1C0
            void *m_pModeStackCapacity;                                     // 0x1C0 - 0x1C8
            uint16_t m_wFlags1C8;                                           // 0x1C8 - 0x1CA
            uint8_t m_bUnk1CA;                                              // 0x1CA - 0x1CB
            uint8_t m_bUnk1CB;                                              // 0x1CB - 0x1CC (init 1)
            uint32_t m_dwUnk1CC;                                            // 0x1CC - 0x1D0
            uint32_t m_dwUnk1D0;                                            // 0x1D0 - 0x1D4 (init 1053609165 = 0.29f)

            // Section 0x1D4 - 0x1F8: Unknown region
            uint8_t m_pad1D4[0x24];                                         // 0x1D4 - 0x1F8

            // Section 0x1F8 - 0x230: Scene objects and pointers
            void *m_pRefCountPtr1;                                          // 0x1F8 - 0x200
            void *m_pSceneObject;                                           // 0x200 - 0x208
            char *m_sCameraObject;                                          // 0x208 - 0x210
            void *m_pCameraComponent;                                       // 0x210 - 0x218 (offset 66*8=528 from this+8)
            void *m_pScene;                                                 // 0x218 - 0x220
            void *m_pRenderContext;                                         // 0x220 - 0x228
            void *m_pRenderDestination;                                     // 0x228 - 0x230

            // Section 0x230 - 0x298: Vectors and OWORDs
            uint8_t m_bUnk230;                                              // 0x230 - 0x231
            uint8_t m_pad231[0x0B];                                         // 0x231 - 0x23C
            sys::math::C_Vector m_vUnk23C;                                  // 0x23C - 0x248
            void *m_pUnk248;                                                // 0x248 - 0x250
            uint64_t m_qwUnk250;                                            // 0x250 - 0x258
            uint64_t m_qwUnk258;                                            // 0x258 - 0x260
            uint32_t m_dwUnk260;                                            // 0x260 - 0x264
            uint32_t m_pad264;                                              // 0x264 - 0x268
            uint8_t m_oword268[0x10];                                       // 0x268 - 0x278 (OWORD)
            uint8_t m_oword278[0x10];                                       // 0x278 - 0x288 (OWORD)
            uint8_t m_oword288[0x10];                                       // 0x288 - 0x298 (OWORD)

            // Section 0x298 - 0x2F8: Forced look state (96 bytes)
            S_ForcedLook m_ForcedLook;                                      // 0x298 - 0x2F8

            // Section 0x2F8 - 0x328: Request vector and flags
            uint32_t m_dwUnk2F8;                                            // 0x2F8 - 0x2FC
            uint32_t m_pad2FC;                                              // 0x2FC - 0x300
            void *m_pRequestsStart;                                         // 0x300 - 0x308
            void *m_pRequestsEnd;                                           // 0x308 - 0x310
            void *m_pRequestsCapacity;                                      // 0x310 - 0x318
            uint32_t m_dwUnk318;                                            // 0x318 - 0x31C
            uint16_t m_wUnk31C;                                             // 0x31C - 0x31E
            uint16_t m_pad31E;                                              // 0x31E - 0x320
            bool m_bInitialized;                                            // 0x320 - 0x321 (set to 1 at offset 800)
            uint8_t m_pad321[0x07];                                         // 0x321 - 0x328

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
