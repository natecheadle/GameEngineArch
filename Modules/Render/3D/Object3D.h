#pragma once

#include "Material/Material.h"
#include "Matrix4x4.h"
#include "VertexPoint3D.hpp"

#include <bgfx/bgfx.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace nate::Modules::Render
{
    class Object3D
    {
        std::vector<VertexPoint3D> m_Points;
        std::vector<std::uint16_t> m_Indices;
        Matrix4x4                  m_Transformation;
        bgfx::IndexBufferHandle    m_IndexBuffer;
        bgfx::VertexBufferHandle   m_VertexBuffer;
        std::shared_ptr<Material>  m_pMaterial;

      public:
        // Cannot create objects until BGFX is initialized.
        Object3D(
            std::vector<VertexPoint3D> points,
            std::vector<std::uint16_t> indices,
            std::shared_ptr<Material>  pMaterial);
        virtual ~Object3D();

        const std::vector<VertexPoint3D>& Points() const { return m_Points; }
        size_t                            PointsSize() const { return m_Points.size() * sizeof(VertexPoint3D); }
        const std::vector<std::uint16_t>& Indices() const { return m_Indices; }
        size_t                            IndecesSize() const { return m_Indices.size() * sizeof(std::uint16_t); }

        const bgfx::IndexBufferHandle&   IndexBufferHandle() const { return m_IndexBuffer; }
        const bgfx::VertexBufferHandle&  VertexBufferHandle() const { return m_VertexBuffer; }
        static const bgfx::VertexLayout& VertexBufferLayout() { return s_VertexLayout; }

        const Material* GetMaterial() const { return m_pMaterial.get(); }

        const Matrix4x4& Transformation() const { return m_Transformation; }
        void             Transformation(const Matrix4x4& value) { m_Transformation = value; }
        void             ResetTransformation() { m_Transformation.SetToIdentity(); }

        void Translate(const Vector3D& value) { m_Transformation.Translate(value); }
        void Rotate(const Vector3D& value) { m_Transformation.Rotate(value); }

      private:
        static bgfx::VertexLayout InitializeVertexLayout();

        static const bgfx::VertexLayout s_VertexLayout;
    };
} // namespace nate::Modules::Render
