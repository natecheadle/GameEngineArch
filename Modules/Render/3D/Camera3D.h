#pragma once

#include "Matrix4x4.h"
#include "Vector3D.hpp"

namespace nate::Modules::Render {
    class Camera3D {
        Matrix4x4 m_View;
        float     m_FieldOfView;
        float     m_Near;
        float     m_Far;

      public:
        Camera3D();

        void             View(const Matrix4x4& value) { m_View = value; }
        const Matrix4x4& View() const { return m_View; }

        float Near() const { return m_Near; }
        void  Near(float value) { m_Near = value; }

        float Far() const { return m_Far; }
        void  Far(float value) { m_Far = value; }

        float FieldOfView() const { return m_FieldOfView; }
        void  FieldOfView(float value) { m_FieldOfView = value; }

        Matrix4x4 CreateProjection(int width, int height) const;

        void Translate(const Vector3D& value) { m_View.Translate(value); }
        void Rotate(const Vector3D& value) { m_View.Rotate(value); }
    };
} // namespace nate::Modules::Render