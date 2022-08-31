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

namespace nate::Modules::Render {
    Camera3D::Camera3D(GUI::IWindow* pWindow)
        : m_FieldOfView(60.0)
        , m_Near(0.1)
        , m_Far(100.0)
        , m_pWindow(pWindow)
    {
        m_View.SetToIdentity();

        SubscribeToMessage(GUI::WindowMessages::WindowResized, [this](const GUI::WindowMessage* pMessage) {
            OnWindowResized(pMessage);
        });
        SubscribeToMessage(GUI::WindowMessages::KeyPressed, [this](const GUI::WindowMessage* pMessage) {
            OnKeyPressed(pMessage);
        });
        SubscribeToMessage(GUI::WindowMessages::MouseClicked, [this](const GUI::WindowMessage* pMessage) {
            OnMouseClicked(pMessage);
        });
    }

    Camera3D::~Camera3D()
    {
        m_pWindow->Unsubscribe(this);
    }

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

    void Camera3D::SubscribeToMessage(
        GUI::WindowMessages                            message,
        std::function<void(const GUI::WindowMessage*)> callback)
    {
        m_pWindow->SubscribeToMessage(this, message, std::move(callback));
    }

    void Camera3D::OnMouseClicked(const GUI::WindowMessage* pMessage)
    {
        const auto* pMouseClick = DebugCast<const GUI::MouseClicked*>(pMessage);
        assert(pMouseClick->GetData());
        OnMouseClicked(*pMouseClick->GetData());
    }

    void Camera3D::OnKeyPressed(const GUI::WindowMessage* pMessage)
    {
        const auto* pKeyPressed = DebugCast<const GUI::KeyPressed*>(pMessage);
        assert(pKeyPressed->GetData());
        OnKeyPressed(*pKeyPressed->GetData());
    }

    void Camera3D::OnWindowResized(const GUI::WindowMessage* pMessage)
    {
        const auto* pResized = DebugCast<const GUI::WindowResized*>(pMessage);
        assert(pResized->GetData());
        OnWindowResized(*pResized->GetData());
    }

} // namespace nate::Modules::Render