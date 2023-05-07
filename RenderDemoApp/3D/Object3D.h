#include "../Shader/ShaderProgram.h"
#include "SquareMatrix4x4.hpp"

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
        Vector3<float>                 m_Origin;
        Vector3<float>                 m_Rotation;

      public:
        Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces);
        ~Object3D();

        void Shader(std::shared_ptr<ShaderProgram> pShader) { m_pShader = std::move(pShader); }
        const std::shared_ptr<ShaderProgram>& Shader() const { return m_pShader; }

        void Draw();

        SquareMatrix4x4<float> ModelMatrix() const;

        void Translate(const Vector3<float>& val) { m_Origin += val; }
        void Rotate(const Vector3<float>& val) { m_Rotation += val; }

        void TranslateX(float val) { m_Origin[0] += val; }
        void TranslateY(float val) { m_Origin[1] += val; }
        void TranslateZ(float val) { m_Origin[2] += val; }

        void RotX(float val) { m_Rotation[0] += val; }
        void RotY(float val) { m_Rotation[1] += val; }
        void RotZ(float val) { m_Rotation[2] += val; }
    };

    static_assert(sizeof(VertexData) == sizeof(float) * 8, "VertexData must be tightly packed");
} // namespace nate::Modules::Render
