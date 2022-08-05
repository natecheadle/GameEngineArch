#include "Object3D.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace nate::Modules::Render {
    const bgfx::VertexLayout Object3D::s_VertexLayout = Object3D::InitializeVertexLayout();

    Object3D::Object3D(std::vector<VertexPoint3D> points, std::vector<std::uint16_t> indices)
        : m_Points(std::move(points))
        , m_Indices(std::move(indices))
        , m_VertexBuffer(
              bgfx::createVertexBuffer(bgfx::makeRef(Points().data(), (uint32_t)PointsSize()), s_VertexLayout))
        , m_IndexBuffer(bgfx::createIndexBuffer(bgfx::makeRef(Indices().data(), IndecesSize())))
    {
        m_Transformation.SetToIdentity();
    }

    Object3D::~Object3D()
    {
        bgfx::destroy(m_VertexBuffer);
        bgfx::destroy(m_IndexBuffer);
    }

    bgfx::VertexLayout Object3D::InitializeVertexLayout()
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        return layout;
    }
} // namespace nate::Modules::Render
