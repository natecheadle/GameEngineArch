#include "Camera3D.h"

#include "KeyPressedInfo.hpp"
#include "Matrix4x4.h"
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
    Camera3D::Camera3D(GUI::IWindow* pWindow)
        : m_pWindow(pWindow)
    {
    }

    Camera3D::~Camera3D() {}
    SquareMatrix4x4<float> Camera3D::Projection() const
    {
        return SquareMatrix4x4<float>::perspective(m_FOV, m_pWindow->GetLastWindowSize().AspectRatio(), m_Near, m_Far);
    }

    SquareMatrix4x4<float> Camera3D::View() const
    {
        return SquareMatrix4x4<float>::lookat(m_Right, m_CameraUp, m_Direction, m_Target);
    }

    void Camera3D::CameraPosition(const Vector3<float>& val)
    {
        m_Position = val;
        Recalculate();
    }

    void Camera3D::CameraTarget(const Vector3<float>& val)
    {
        m_Target = val;
        Recalculate();
    }

    void Camera3D::WorldUp(const Vector3<float>& val)
    {
        m_WorldUp = val;
        Recalculate();
    }

    void Camera3D::Translate(const Vector3<float>& value)
    {
        m_Position += value;
        Recalculate();
    }
    void Camera3D::RotateX(float val)
    {
        SquareMatrix4x4<float> rotMat(SquareMatrix4x4<float>::rotate_x_init(val));
        m_Direction = (rotMat * m_Direction).ToVector3();
        m_Direction.normalize_this();
        m_Target = m_Direction - m_Position;
        Recalculate();
    }
    void Camera3D::RotateY(float val)
    {
        SquareMatrix4x4<float> rotMat(SquareMatrix4x4<float>::rotate_y_init(val));
        m_Direction = (rotMat * m_Direction).ToVector3();
        m_Direction.normalize_this();
        m_Target = m_Direction - m_Position;
        Recalculate();
    }
    void Camera3D::RotateZ(float val)
    {
        SquareMatrix4x4<float> rotMat(SquareMatrix4x4<float>::rotate_z_init(val));
        m_Direction = (rotMat * m_Direction).ToVector3();
        m_Direction.normalize_this();
        m_Target = m_Direction - m_Position;
        Recalculate();
    }

    void Camera3D::Recalculate()
    {
        m_Direction = Vector3<float>(m_Position - m_Target).normalize_this();
        m_Right     = m_WorldUp.cross(m_Direction).normalize_this();
        m_CameraUp  = m_Direction.cross(m_Right).normalize_this();
    }
} // namespace nate::Modules::Render