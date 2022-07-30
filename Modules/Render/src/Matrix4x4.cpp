#include "Matrix4x4.h"

#include <bx/math.h>

namespace nate::Modules::Render {
    Matrix4x4::Matrix4x4()
    {
        Clear();
    }

    Matrix4x4::Matrix4x4(const MatrixData& rawData)
        : m_Data(rawData)
    {
    }

    void Matrix4x4::SetToIdentity()
    {
        bx::mtxIdentity(m_Data.data());
    }

    void Matrix4x4::Clear()
    {
        m_Data.fill(0.0);
    }

    void Matrix4x4::Rotate(float pitch, float yaw, float roll)
    {
        MatrixData value;
        bx::mtxRotateXYZ(value.data(), pitch, yaw, roll);
        Multiply(value);
    }

    void Matrix4x4::Rotate(const Vector3D& value)
    {
        Rotate(value.X(), value.Y(), value.Z());
    }

    void Matrix4x4::Translate(float x, float y, float z)
    {
        MatrixData value;
        bx::mtxTranslate(value.data(), x, y, z);
        Multiply(value);
    }

    void Matrix4x4::Translate(const Vector3D& value)
    {
        Translate(value.X(), value.Y(), value.Z());
    }

    void Matrix4x4::Multiply(const Matrix4x4& other)
    {
        MatrixData value;
        bx::mtxMul(value.data(), other.m_Data.data(), m_Data.data());
        m_Data = value;
    }

    void Matrix4x4::Invert()
    {
        MatrixData value;
        bx::mtxInverse(value.data(), m_Data.data());
        m_Data = value;
    }
} // namespace nate::Modules::Render