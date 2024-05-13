#pragma once

#include "Camera.h"

namespace Ignosi::Libraries::Renderer
{
    class Camera2D : public Camera
    {
        float m_ZoomSpeed{0.1};
        float m_PanSpeed{0.1};

        Math::Radian<float> m_Pitch;
        Math::Radian<float> m_Yaw;
        Math::Radian<float> m_MaxRot{M_PI_2};

      public:
        Camera2D(IWindow* pWindow);
        virtual ~Camera2D() = default;

        float ZoomSpeed() const { return m_ZoomSpeed; }
        float PanSpeed() const { return m_PanSpeed; }

        void ZoomSpeed(float value) { m_ZoomSpeed = value; }
        void PanSpeed(float value) { m_PanSpeed = value; }

        void PanUp(float value);
        void PanDown(float value);
        void PanLeft(float value);
        void PanRight(float value);

        void ZoomIn(float value);
        void ZoomOut(float value);

      private:
        void PanUp() { PanUp(m_PanSpeed); }
        void PanDown() { PanDown(m_PanSpeed); }
        void PanLeft() { PanLeft(m_PanSpeed); }
        void PanRight() { PanRight(m_PanSpeed); }

        void ZoomIn() { ZoomIn(m_ZoomSpeed); }
        void ZoomOut() { ZoomOut(m_ZoomSpeed); }
    };
} // namespace Ignosi::Libraries::Renderer
