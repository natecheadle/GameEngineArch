#pragma once

#include "Matrix4x4.h"
#include "VertexPoint3D.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace nate::Modules::Render {
    class Object3D {
        std::vector<VertexPoint3D> m_Points;
        std::vector<std::uint16_t> m_Indices;
        Matrix4x4                  m_Transformation;

      public:
        Object3D(std::vector<VertexPoint3D> points, std::vector<std::uint16_t> indices);

        const std::vector<VertexPoint3D>& Points() const { return m_Points; }
        size_t                            PointsSize() const { return m_Points.size() * sizeof(VertexPoint3D); }
        const std::vector<std::uint16_t>& Indices() const { return m_Indices; }
        size_t                            IndecesSize() const { return m_Indices.size() * sizeof(std::uint16_t); }

        const Matrix4x4& Transformation() const { return m_Transformation; }
        void             Transformation(const Matrix4x4& value) { m_Transformation = value; }
        void             ResetTransformation() { m_Transformation.SetToIdentity(); }

        void Translate(const Vector3D& value) { m_Transformation.Translate(value); }
        void Rotate(const Vector3D& value) { m_Transformation.Rotate(value); }
    };
} // namespace nate::Modules::Render
