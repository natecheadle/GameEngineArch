#include "Camera3D.h"

#include "KeyPressedInfo.hpp"
#include "Matrix4x4.h"

#include <DebugCast.hpp>
#include <IWindow.h>
#include <Messages/KeyPressed.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <bgfx/bgfx.h>
#include <bx/math.h>

#include <cassert>
#include <chrono>

namespace nate::Modules::Render
{
    Camera3D::Camera3D(GUI::IWindow* pWindow)
        : m_FieldOfView(60.0)
        , m_Near(0.1)
        , m_Far(100.0)
        , m_pWindow(pWindow)
    {
        m_View.SetToIdentity();
    }

    Camera3D::~Camera3D() {}

    Matrix4x4 Camera3D::CreateProjection(int width, int height) const
    {
        Matrix4x4 proj;
        bx::mtxProj(
            proj.Data().data(),
            m_FieldOfView,
            float(width) / float(height),
            m_Near,
            m_Far,
            bgfx::getCaps()->homogeneousDepth);

        return proj;
    }
} // namespace nate::Modules::Render