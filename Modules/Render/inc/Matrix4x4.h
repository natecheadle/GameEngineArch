#pragma once

#include "../3D/Vector3D.hpp"

#include <array>

namespace nate::Modules::Render {

    using MatrixData = std::array<float, 16>;
    class Matrix4x4 {

        MatrixData m_Data;

      public:
        Matrix4x4();
        Matrix4x4(const MatrixData& rawData);

        void SetToIdentity();
        void Clear();

        MatrixData& Data() { return m_Data; }

        void Rotate(float pitch, float yaw, float roll);
        void Rotate(const Vector3D& value);
        void Translate(float x, float y, float z);
        void Translate(const Vector3D& value);

        void Multiply(const Matrix4x4& other);
        void Invert();
    };
} // namespace nate::Modules::Render
