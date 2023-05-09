#include "../Shader/ShaderProgram.h"
#include "../Texture/Texture.h"
#include "SquareMatrix4x4.hpp"

#include <Vector3.hpp>

#include <initializer_list>
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
        std::vector<VertexData>               m_Vertexes;
        std::vector<std::uint32_t>            m_Indeces;
        std::shared_ptr<ShaderProgram>        m_pShader;
        Vector3<float>                        m_Origin;
        Vector3<Radian<float>>                m_Rotation;
        std::vector<std::shared_ptr<Texture>> m_Textures;

      protected:
        Object3D(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces);
        Object3D(std::vector<VertexData> vertexes);

      public:
        virtual ~Object3D();

        const std::vector<VertexData>&    Vertexes() const { return m_Vertexes; }
        const std::vector<std::uint32_t>& Indeces() const { return m_Indeces; }

        void Textures(std::vector<std::shared_ptr<Texture>> val) { m_Textures = std::move(val); }
        const std::vector<std::shared_ptr<Texture>>& Textures() const { return m_Textures; }

        void Shader(std::shared_ptr<ShaderProgram> pShader) { m_pShader = std::move(pShader); }
        const std::shared_ptr<ShaderProgram>& Shader() const { return m_pShader; }

        virtual void Draw();

        SquareMatrix4x4<float> ModelMatrix() const;

        void Translate(const Vector3<float>& val) { m_Origin += val; }
        void Rotate(const Vector3<Radian<float>>& val) { m_Rotation += val; }

        void TranslateX(float val) { m_Origin[0] += val; }
        void TranslateY(float val) { m_Origin[1] += val; }
        void TranslateZ(float val) { m_Origin[2] += val; }

        void RotX(const Radian<float>& val) { m_Rotation[0] += val; }
        void RotY(const Radian<float>& val) { m_Rotation[1] += val; }
        void RotZ(const Radian<float>& val) { m_Rotation[2] += val; }
    };

    static_assert(sizeof(VertexData) == sizeof(float) * 8, "VertexData must be tightly packed");
} // namespace nate::Modules::Render
