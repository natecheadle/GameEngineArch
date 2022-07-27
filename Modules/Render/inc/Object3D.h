
#include "VertexPoint.hpp"

#include <vector>

namespace nate::Modules::Render {
    class Object3D {
        std::vector<VertexPoint> m_Points;
        std::vector<size_t>      m_Indexes;

      public:
        Object3D(std::vector<VertexPoint> points, std::vector<size_t> indexes)
            : m_Points(std::move(points))
            , m_Indexes(std::move(indexes))
        {
        }

        const std::vector<VertexPoint>& Points() const { return m_Points; }
        const std::vector<size_t>&      Indexes() const { return m_Indexes; }
    };
} // namespace nate::Modules::Render