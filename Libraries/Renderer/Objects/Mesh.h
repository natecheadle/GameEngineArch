#pragma once

#include "IVertexBuffer.h"
#include "Shader/IShaderProgram.h"
#include "Texture/IMaterial.h"
#include "Units/Radian.hpp"
#include "VertexData.h"

namespace Ignosi::Libraries::Renderer
{
    class IRenderer;

    class Mesh
    {
        static const VertexData s_CubePoints[];
        static const VertexData s_SpritePoints[];

        const IVertexBuffer*  m_Vertexes;
        const IShaderProgram* m_Shader;
        const IMaterial*      m_Material;

        Math::Vector3<float>               m_Translation;
        Math::Vector3<Math::Radian<float>> m_Rotation;

      public:
        Mesh(const IVertexBuffer* vertexes, const IShaderProgram* shader, const IMaterial* material);
        virtual ~Mesh() = default;

        Mesh(const Mesh& other) = default;
        Mesh(Mesh&& other)      = default;

        Mesh& operator=(const Mesh& other) = default;
        Mesh& operator=(Mesh&& other)      = default;

        const IVertexBuffer*  Vertexes() const { return m_Vertexes; }
        const IShaderProgram* Shader() const { return m_Shader; }
        const IMaterial*      Material() const { return m_Material; }

        const Math::Vector3<float>&               Translation() const { return m_Translation; }
        const Math::Vector3<Math::Radian<float>>& Rotation() const { return m_Rotation; }

        void Translation(const Math::Vector3<float>& value) { m_Translation = value; }
        void Rotation(const Math::Vector3<Math::Radian<float>>& value) { m_Rotation = value; }

        static std::unique_ptr<IVertexBuffer> CreateCubeVertexes(const IRenderer* pRenderer);
        static std::unique_ptr<IVertexBuffer> CreateSpriteVertexes(const IRenderer* pRenderer);

        Math::SquareMatrix4x4<float> ModelMatrix(
            const Math::Vector3<float>&              origin,
            const Math::Vector3<Math::Radian<float>> origin_rotation) const;
        Math::SquareMatrix3x3<float> NormalMatrix(const Math::SquareMatrix4x4<float>& model) const;
    };
} // namespace Ignosi::Libraries::Renderer
