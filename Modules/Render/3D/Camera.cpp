#include "Camera.h"

#include "KeyPressedInfo.hpp"
#include "SquareMatrix4x4.hpp"

#include <DebugCast.hpp>
#include <IWindow.h>
#include <Messages/KeyPressed.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>

#include <cassert>
#include <chrono>

namespace nate::Modules::Render
{
    Camera::Camera(GUI::IWindow* pWindow)
        : m_pWindow(pWindow)
    {
    }

    Camera::~Camera() {}

    SquareMatrix4x4<float> Camera::Projection() const
    {
        return SquareMatrix4x4<float>::perspective(m_FOV, m_pWindow->GetLastWindowSize().AspectRatio(), m_Near, m_Far);
    }

    SquareMatrix4x4<float> Camera::View() const
    {
        auto right = m_Direction.cross(m_WorldUp).normalize_this();
        auto up    = right.cross(m_Direction).normalize_this();
        return SquareMatrix4x4<float>::lookat(m_Position, m_Position + m_Direction, up);
    }

    void Camera::CameraPosition(const Vector3<float>& val)
    {
        m_Position = val;
    }

    void Camera::CameraDirection(const Vector3<float>& val)
    {
        m_Direction = val.normalize();
    }

    void Camera::WorldUp(const Vector3<float>& val)
    {
        m_WorldUp = val;
    }

    void Camera::Translate(const Vector3<float>& value)
    {
        m_Position += value;
    }

} // namespace nate::Modules::Render