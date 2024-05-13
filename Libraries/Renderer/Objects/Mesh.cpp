#include "Mesh.h"

namespace Ignosi::Libraries::Renderer
{
    Mesh::Mesh(std::shared_ptr<IVertexBuffer> vertexes, std::shared_ptr<IShaderProgram> shader, std::shared_ptr<IMaterial> material)
        : m_Vertexes(std::move(vertexes))
        , m_Shader(std::move(shader))
        , m_Material(std::move(material))
    {
        assert(m_Vertexes.get());
        assert(m_Shader.get());
    }
} // namespace Ignosi::Libraries::Renderer
