#pragma once

#include "Color.hpp"
#include "Vector3D.hpp"

namespace nate::Modules::Render {

    class VertexPoint3D {
      private:
        Vector3D m_Point;
        Colour   m_Color;

      public:
        VertexPoint3D(Vector3D point, Colour color) noexcept
            : m_Point(point)
            , m_Color(color)
        {
            static_assert(
                sizeof(VertexPoint3D) == (3 * sizeof(float) + sizeof(std::uint32_t)),
                "VertexPoint3D must be packed tightly");
        }

        VertexPoint3D()
        {
            static_assert(
                sizeof(VertexPoint3D) == (3 * sizeof(float) + sizeof(std::uint32_t)),
                "VertexPoint3D must be packed tightly");
        }

        const Vector3D& Point() const { return m_Point; }
        Colour          Color() const { return m_Color; }

        void Point(const Vector3D& value) { m_Point = value; }
        void Color(Colour value) { m_Color = value; }
    };
} // namespace nate::Modules::Render