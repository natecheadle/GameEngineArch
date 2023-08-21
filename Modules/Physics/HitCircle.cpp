#include "HitCircle.h"

#include "LinearAlgebra/Vector2.hpp"

#include <3D/GenericVertexes.hpp>
#include <LinearAlgebra/SquareMatrix4x4.hpp>

#include <vector>

namespace Ignosi::Modules::Physics
{
    namespace
    {
        constexpr size_t VertCount    = 16;
        constexpr auto   CIRCLE_VERTS = Render::GenerateCircleVertexes<VertCount>();
        const float*     circle_begin = CIRCLE_VERTS.data()->data();
        const size_t     circle_size  = sizeof(CIRCLE_VERTS) / sizeof(float);
        const float*     circle_end   = circle_begin + circle_size;
    } // namespace

    HitCircle::HitCircle(Render::Renderer* pRenderer)
        : HitShape(pRenderer)
    {
    }

    std::span<const float> HitCircle::VertexData() const
    {
        return std::span<const float>(circle_begin, circle_end);
    }
} // namespace Ignosi::Modules::Physics
