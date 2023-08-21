#include "HitRectangle.h"

#include "HitShape.h"
#include "LinearAlgebra/SquareMatrix.hpp"
#include "LinearAlgebra/Vector2.hpp"
#include "Units/Radian.hpp"

#include <3D/GenericVertexes.hpp>

#include <cmath>
#include <vector>

namespace Ignosi::Modules::Physics
{
    namespace
    {
        const float* rect_begin = Render::RectangleVertexes.data()->data();
        const size_t rect_size  = sizeof(Render::RectangleVertexes) / sizeof(float);
        const float* rect_end   = rect_begin + rect_size;
    } // namespace

    HitRectangle::HitRectangle(Render::Renderer* pRenderer)
        : HitShape(pRenderer)
    {
        m_TestAxes.resize(2);
        m_Corners.resize(4);

        UpdatePrivateVectors();
    }

    void HitRectangle::Width(float value)
    {
        m_Width  = value;
        m_Radius = std::sqrt(m_Height * m_Height + m_Width * m_Width);
        UpdatePrivateVectors();
    }

    void HitRectangle::Height(float value)
    {
        m_Height = value;
        m_Radius = std::sqrt(m_Height * m_Height + m_Width * m_Width);
        UpdatePrivateVectors();
    }

    void HitRectangle::Rotation(const Radian<float>& value)
    {
        HitShape::Rotation(value);
        UpdatePrivateVectors();
    }

    std::span<const float> HitRectangle::VertexData() const
    {
        return std::span<const float>(rect_begin, rect_end);
    }

    void HitRectangle::UpdatePrivateVectors()
    {
        Radian<float> rotation = HitShape::Rotation();
        float         cos_rot  = cos(rotation);
        float         sin_rot  = sin(rotation);

        SquareMatrix<2, float> rotMat;
        rotMat[0][0] = cos_rot;
        rotMat[0][1] = sin_rot;
        rotMat[1][0] = -sin_rot;
        rotMat[1][1] = cos_rot;

        m_TestAxes[0] = (rotMat * Vector2<float>(1, 0)).normalize_this();
        m_TestAxes[1] = (rotMat * Vector2<float>(0, 1)).normalize_this();

        const float x = m_Width / 2.0f;
        const float y = m_Height / 2.0f;

        m_Corners[0] = rotMat * (Origin() + Vector2<float>(x, y));
        m_Corners[1] = rotMat * (Origin() + Vector2<float>(x, -y));
        m_Corners[2] = rotMat * (Origin() + Vector2<float>(-x, -y));
        m_Corners[3] = rotMat * (Origin() + Vector2<float>(-x, y));
    }
} // namespace Ignosi::Modules::Physics
