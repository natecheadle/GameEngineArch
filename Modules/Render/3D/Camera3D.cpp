#include "Camera3D.h"

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
    Camera3D::Camera3D(GUI::IWindow* pWindow)
        : m_pWindow(pWindow)
    {
        Recalculate();
    }

    Camera3D::~Camera3D() {}

    SquareMatrix4x4<float> Camera3D::Projection() const
    {
        return SquareMatrix4x4<float>::perspective(m_FOV, m_pWindow->GetLastWindowSize().AspectRatio(), m_Near, m_Far);
    }

    SquareMatrix4x4<float> Camera3D::View() const
    {
        return SquareMatrix4x4<float>::lookat(m_Position, m_Target, m_WorldUp);
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
        m_Target += value;
        Recalculate();
    }
    void Camera3D::RotateX(float val)
    {
        // TODO I think this is wrong.
        SquareMatrix4x4<float> rotMat(SquareMatrix4x4<float>::rotate_x_init(val));
        m_Direction = (rotMat * m_Direction).ToVector3();
        m_Direction.normalize_this();
        m_Target = m_Direction - m_Position;
        Recalculate();
    }
    void Camera3D::RotateY(float val)
    {
        // TODO I think this is wrong.
        SquareMatrix4x4<float> rotMat(SquareMatrix4x4<float>::rotate_y_init(val));
        m_Direction = (rotMat * m_Direction).ToVector3();
        m_Direction.normalize_this();
        m_Target = m_Direction - m_Position;
        Recalculate();
    }
    void Camera3D::RotateZ(float val)
    {
        // TODO I think this is wrong.
        SquareMatrix4x4<float> rotMat(SquareMatrix4x4<float>::rotate_z_init(val));
        m_Direction = (rotMat * m_Direction).ToVector3();
        m_Direction.normalize_this();
        m_Target = m_Direction - m_Position;
        Recalculate();
    }

    void Camera3D::Recalculate()
    {
        m_Direction = Vector3<float>(m_Position - m_Target).normalize_this();
    }
} // namespace nate::Modules::Render