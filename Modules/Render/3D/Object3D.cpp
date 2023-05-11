#include "Object3D.h"

#include "SquareMatrix4x4.hpp"
#include "Vector3.hpp"

#include <algorithm>
#include <cstddef>

namespace nate::Modules::Render
{
    Object3D::Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces)
        : m_Vertexes(std::move(vertexes))
        , m_Indeces(std::move(indeces))
    {
    }

    Object3D::Object3D(std::vector<VertexData> vertexes)
        : m_Vertexes(std::move(vertexes))
    {
    }

    SquareMatrix4x4<float> Object3D::ModelMatrix() const
    {
        if (m_Rotation == Vector3<Radian<float>>({0.0, 0.0, 0.0}) && m_Origin == Vector3<float>({0.0, 0.0, 0.0}))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation));
        rslt *= SquareMatrix4x4<float>::translate_init(m_Origin);
        return rslt;
    }

    void Object3D::Draw()
    {
        for (auto& tex : m_Textures)
        {
            if (tex)
            {
                tex->Activate();
                tex->Bind();
            }
        }
        m_pShader->Use();
    }
} // namespace nate::Modules::Render
