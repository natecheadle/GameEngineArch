#pragma once

#include "IVertexBuffer.h"
#include "Shader/IShaderProgram.h"
#include "Texture/IMaterial.h"

#include <memory>

namespace Ignosi::Libraries::Renderer
{
    class Mesh
    {
        const IVertexBuffer*  m_Vertexes;
        const IShaderProgram* m_Shader;
        const IMaterial*      m_Material;

      public:
        Mesh(const IVertexBuffer* vertexes, const IShaderProgram* shader, const IMaterial* material);
        virtual ~Mesh() = default;

        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&& other)      = default;

        Mesh& operator=(const Mesh& other) = delete;
        Mesh& operator=(Mesh&& other)      = default;

        const IVertexBuffer*  Vertexes() const { return m_Vertexes; }
        const IShaderProgram* Shader() const { return m_Shader; }
        const IMaterial*      Material() const { return m_Material; }
    };
} // namespace Ignosi::Libraries::Renderer
