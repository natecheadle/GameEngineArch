#pragma once

#include "3D/Vector3D.hpp"
#include "IWindow.h"
#include "KeyPressedInfo.hpp"
#include "Keys.h"
#include "Matrix4x4.h"
#include "MouseClickedInfo.hpp"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <chrono>
#include <functional>

namespace nate::Modules::Render
{
    class Camera3D
    {
        Matrix4x4     m_View;
        float         m_FieldOfView;
        float         m_Near;
        float         m_Far;
        GUI::IWindow* m_pWindow;

      public:
        virtual ~Camera3D();

        Matrix4x4 CreateProjection(int width, int height) const;

        const Matrix4x4& View() const { return m_View; }

        void Translate(const Vector3D& value) { m_View.Translate(value); }
        void Rotate(const Vector3D& value) { m_View.Rotate(value); }

        virtual void Update(std::chrono::nanoseconds /* time */) {}

      protected:
        Camera3D(GUI::IWindow* pWindow);

        void View(const Matrix4x4& value) { m_View = value; }

        float Near() const { return m_Near; }
        void  Near(float value) { m_Near = value; }

        float Far() const { return m_Far; }
        void  Far(float value) { m_Far = value; }

        float FieldOfView() const { return m_FieldOfView; }
        void  FieldOfView(float value) { m_FieldOfView = value; }

        const GUI::IWindow* Window() const { return m_pWindow; }
    };
} // namespace nate::Modules::Render