#include "Camera.h"

#include <LinearAlgebra/SquareMatrix.hpp>

#include <cassert>

namespace Ignosi::Libraries::Renderer
{
    Camera::Camera(IWindow* pWindow)
        : m_pWindow(pWindow)
    {
    }

    Camera::~Camera() {}

    Math::SquareMatrix4x4<float> Camera::Projection() const
    {
        return Math::SquareMatrix4x4<float>::perspective(m_FOV, m_pWindow->ActualWindowSize().AspectRatio(), m_Near, m_Far);
    }

    Math::SquareMatrix4x4<float> Camera::ViewPerspective() const
    {
        Math::Vector3<float> right = m_Direction.cross(m_WorldUp).normalize_this();
        auto                 up    = right.cross(m_Direction).normalize_this();
        return Math::SquareMatrix4x4<float>::lookat(m_Position, m_Position + m_Direction, up);
    }

    Math::SquareMatrix4x4<float> Camera::ViewOrthographic() const
    {
        auto win_size = m_pWindow->ActualWindowSize();
        return Math::SquareMatrix4x4<float>::orthographic(
            0.0f,
            static_cast<float>(win_size.Width),
            static_cast<float>(win_size.Height),
            0.0,
            m_Near,
            m_Far);
    }

    void Camera::CameraPosition(const Math::Vector3<float>& val)
    {
        m_Position = val;
    }

    void Camera::CameraDirection(const Math::Vector3<float>& val)
    {
        m_Direction = val.normalize();
    }

    void Camera::WorldUp(const Math::Vector3<float>& val)
    {
        m_WorldUp = val;
    }

    void Camera::Translate(const Math::Vector3<float>& value)
    {
        m_Position += value;
    }

} // namespace Ignosi::Libraries::Renderer
