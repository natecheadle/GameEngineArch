#include "Mesh.h"

namespace Ignosi::Libraries::Renderer
{
    Mesh::Mesh(const IVertexBuffer* vertexes, const IShaderProgram* shader, const IMaterial* material)
        : m_Vertexes(vertexes)
        , m_Shader(shader)
        , m_Material(material)
    {
        assert(m_Vertexes);
        assert(m_Shader);
    }
} // namespace Ignosi::Libraries::Renderer
