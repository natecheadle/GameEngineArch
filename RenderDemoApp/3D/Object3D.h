#include "../Shader/ShaderProgram.h"

#include <Vector3.hpp>

#include <memory>
#include <vector>

namespace nate::Modules::Render
{
    struct VertexData
    {
        Vector3<float> Position;
        Vector3<float> Color;
        Vector2<float> TextureCoord;
    };

    class Object3D
    {
        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_EBO;

        std::vector<VertexData>        m_Vertexes;
        std::vector<std::uint32_t>     m_Indeces;
        std::shared_ptr<ShaderProgram> m_pShader;

      public:
        Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces);
        ~Object3D();

        void Shader(std::shared_ptr<ShaderProgram> pShader) { m_pShader = std::move(pShader); }
        const std::shared_ptr<ShaderProgram>& Shader() const { return m_pShader; }

        void Draw();
    };

    static_assert(sizeof(VertexData) == sizeof(float) * 8, "VertexData must be tightly packed");
} // namespace nate::Modules::Render
