#pragma once

#include "IVertexBuffer.h"
#include "Shader/IShaderProgram.h"
#include "Texture/IMaterial.h"

#include <memory>

namespace Ignosi::Libraries::Renderer
{
    class Mesh
    {
        std::shared_ptr<IVertexBuffer>  m_Vertexes;
        std::shared_ptr<IShaderProgram> m_Shader;
        std::shared_ptr<IMaterial>      m_Material;

      public:
        Mesh(std::shared_ptr<IVertexBuffer> vertexes, std::shared_ptr<IShaderProgram> shader, std::shared_ptr<IMaterial> material);
        virtual ~Mesh() = default;

        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&& other)      = default;

        Mesh& operator=(const Mesh& other) = delete;
        Mesh& operator=(Mesh&& other)      = default;

        IVertexBuffer*  Vertexes() const { return m_Vertexes.get(); }
        IShaderProgram* Shader() const { return m_Shader.get(); }
        IMaterial*      Material() const { return m_Material.get(); }
    };
} // namespace Ignosi::Libraries::Renderer
