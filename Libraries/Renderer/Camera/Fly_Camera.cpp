#include "Fly_Camera.h"

#include <LinearAlgebra/Vector3.hpp>
#include <Units/Angle.hpp>

namespace Ignosi::Libraries::Renderer
{
    Fly_Camera::Fly_Camera(IWindow* pWindow)
        : Camera(pWindow)
        , m_Pitch(0.0)
        , m_Yaw(-M_PI_2)
    {
    }

    void Fly_Camera::PanUp(float value)
    {
        Translate({0, -value, 0});
    }

    void Fly_Camera::PanDown(float value)
    {
        Translate({0, value, 0});
    }

    void Fly_Camera::PanLeft(float value)
    {
        Translate({value, 0, 0});
    }

    void Fly_Camera::PanRight(float value)
    {
        Translate({-value, 0, 0});
    }

    void Fly_Camera::RotatePitch(const Math::Radian<float>& value)
    {
        m_Pitch += value;
        m_Pitch = m_Pitch >= m_MaxRot ? m_MaxRot : m_Pitch;
        CameraDirection(CalcDir());
    }

    void Fly_Camera::RotateYaw(const Math::Radian<float>& value)
    {
        m_Yaw += value;
        m_Yaw = m_Yaw >= m_MaxRot ? m_MaxRot : m_Yaw;
        CameraDirection(CalcDir());
    }

    void Fly_Camera::ZoomIn(float value)
    {
        Translate({0, 0, value});
    }

    void Fly_Camera::ZoomOut(float value)
    {
        Translate({0, 0, -value});
    }

    Math::Vector3<float> Fly_Camera::CalcDir() const
    {
        Math::Vector3<float> dir;
        dir.x(cos(m_Yaw) * cos(m_Pitch));
        dir.y(sin(m_Pitch));
        dir.z(sin(m_Yaw) * cos(m_Pitch));

        return dir;
    }

} // namespace Ignosi::Libraries::Renderer
