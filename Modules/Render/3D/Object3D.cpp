#include "Object3D.h"

#include <bx/math.h>

namespace nate::Modules::Render {
    Object3D::Object3D(std::vector<VertexPoint3D> points, std::vector<std::uint16_t> indices)
        : m_Points(std::move(points))
        , m_Indices(std::move(indices))
    {
    }
} // namespace nate::Modules::Render
