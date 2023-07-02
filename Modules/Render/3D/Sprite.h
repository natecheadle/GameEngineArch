#pragma once

#include "3D/Material.h"
#include "3D/RGB_Color.h"
#include "Renderer/VertexBuffer.h"
#include "Shader/ShaderProgram.h"
#include "SpriteVertexData.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector.hpp>
#include <Units/Radian.hpp>

#include <memory>
#include <span>

namespace Ignosi::Modules::Render
{
    class Renderer;

    class Sprite
    {

        Renderer*                     m_pRenderer;
        Vector2<float>                m_Origin{0.0, 0.0};
        Vector2<float>                m_Size{1.0, 1.0};
        Radian<float>                 m_Rotation{0.0};
        std::shared_ptr<VertexBuffer> m_pBuffer;
        std::shared_ptr<Material>     m_pMaterial;
        RGB_Color                     m_Color;

        static const SpriteVertexData m_BasicSpritePoints[];

      public:
        Sprite(Renderer* pRenderer);
        Sprite(
            Renderer*                pRenderer,
            const VertexDataConfig&  config,
            std::span<float>         vertexes,
            std::span<std::uint32_t> indeces);

        Sprite(Renderer* pRenderer, const VertexDataConfig& config, std::span<float> vertexes);

        Sprite(const Sprite& other);
        Sprite(Sprite&& other) = default;

        virtual ~Sprite() = default;

        void AttachedMaterial(std::shared_ptr<Material> pMat) { m_pMaterial = std::move(pMat); }
        const std::shared_ptr<Material>& AttachedMaterial() const { return m_pMaterial; }

        virtual void Draw(ShaderProgram* pShader);
        virtual void Draw();

        SquareMatrix4x4<float> ModelMatrix() const;
        SquareMatrix3x3<float> NormalMatrix() const;

        void             Color(const RGB_Color& val) { m_Color = val; }
        const RGB_Color& Color() const { return m_Color; }

        void                  Origin(const Vector2<float>& val) { m_Origin = val; }
        const Vector2<float>& Origin() const { return m_Origin; }

        const Vector2<float>& Size() const { return m_Size; }
        void                  Size(const Vector2<float>& val) { m_Size = val; }

        void                 Rotation(const Radian<float>& val) { m_Rotation = val; }
        const Radian<float>& Rotation() const { return m_Rotation; }

        void Translate(const Vector2<float>& val) { m_Origin += val; }
        void Rotate(const Radian<float>& val) { m_Rotation += val; }

        void SizeX(float val) { m_Size[0] = val; }
        void SizeY(float val) { m_Size[1] = val; }
        void TranslateX(float val) { m_Origin[0] += val; }
        void TranslateY(float val) { m_Origin[1] += val; }
    };
} // namespace Ignosi::Modules::Render
