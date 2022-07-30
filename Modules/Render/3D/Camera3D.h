#pragma once

#include "Matrix4x4.h"
#include "Vector3D.hpp"

namespace nate::Modules::Render {
    class Camera3D {
        Matrix4x4 m_View;
        Matrix4x4 m_Projection;

      public:
        Camera3D();

        void             View(const Matrix4x4& value) { m_View = value; }
        const Matrix4x4& View() const { return m_View; }

        void             Projection(const Matrix4x4& value) { m_Projection = value; }
        const Matrix4x4& Projection() const { return m_Projection; }

        void Translate(const Vector3D& value) { m_View.Translate(value); }
        void Rotate(const Vector3D& value) { m_View.Rotate(value); }
    };
} // namespace nate::Modules::Render