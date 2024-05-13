#pragma once

#include "Camera.h"

namespace Ignosi::Libraries::Renderer
{
    class Fly_Camera : public Camera
    {
        float m_ZoomSpeed{0.1};
        float m_PanSpeed{0.1};
        float m_RotateSpeed{M_PI_2 / 100.0};

        Math::Radian<float> m_Pitch;
        Math::Radian<float> m_Yaw;
        Math::Radian<float> m_MaxRot{M_PI_2};

      public:
        Fly_Camera(IWindow* pWindow);
        virtual ~Fly_Camera() = default;

        float ZoomSpeed() const { return m_ZoomSpeed; }
        float PanSpeed() const { return m_PanSpeed; }
        float RotateSpeed() const { return m_RotateSpeed; }

        void ZoomSpeed(float value) { m_ZoomSpeed = value; }
        void PanSpeed(float value) { m_PanSpeed = value; }
        void RotateSpeed(float value) { m_RotateSpeed = value; }

        void PanUp(float value);
        void PanDown(float value);
        void PanLeft(float value);
        void PanRight(float value);

        void RotatePitch(const Math::Radian<float>& value);
        void RotateYaw(const Math::Radian<float>& value);

        void ZoomIn(float value);
        void ZoomOut(float value);

      private:
        Math::Vector3<float> CalcDir() const;

        void PanUp() { PanUp(m_PanSpeed); }
        void PanDown() { PanDown(m_PanSpeed); }
        void PanLeft() { PanLeft(m_PanSpeed); }
        void PanRight() { PanRight(m_PanSpeed); }

        void RotatePitchPos() { RotatePitch(m_RotateSpeed); }
        void RotatePitchNeg() { RotatePitch(-m_RotateSpeed); }
        void RotateYawPos() { RotateYaw(m_RotateSpeed); }
        void RotateYawNeg() { RotateYaw(-m_RotateSpeed); }

        void ZoomIn() { ZoomIn(m_ZoomSpeed); }
        void ZoomOut() { ZoomOut(m_ZoomSpeed); }
    };
} // namespace Ignosi::Libraries::Renderer
