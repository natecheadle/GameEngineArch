#pragma once

#include "3D/Material.h"
#include "3D/RGB_Color.h"
#include "KinematicData.h"
#include "LinearAlgebra/Vector3.hpp"
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

        Renderer*                     m_pRenderer{nullptr};
        Physics::KinematicData*       m_pPosition;
        Vector3<float>                m_Translation{0.0, 0.0};
        Vector2<float>                m_Size{1.0, 0.0};
        Radian<float>                 m_Rotation{0.0};
        std::shared_ptr<VertexBuffer> m_pBuffer;
        std::shared_ptr<Material>     m_pMaterial;
        RGB_Color                     m_Color{1.0, 1.0, 1.0};

        static const SpriteVertexData m_BasicSpritePoints[];

      public:
        Sprite() = default;
        Sprite(Renderer* pRenderer, Physics::KinematicData* pPosition);
        Sprite(
            Renderer*                pRenderer,
            Physics::KinematicData*  pPosition,
            const VertexDataConfig&  config,
            std::span<float>         vertexes,
            std::span<std::uint32_t> indeces);
        Sprite(Renderer* pRenderer, Physics::KinematicData* pPosition, const VertexDataConfig& config, std::span<const float> vertexes);

        virtual ~Sprite() = default;

        Sprite(const Sprite& other);
        Sprite(Sprite&& other) = default;

        Sprite& operator=(const Sprite& other)     = default;
        Sprite& operator=(Sprite&& other) noexcept = default;

        void                             AttachedMaterial(std::shared_ptr<Material> pMat) { m_pMaterial = std::move(pMat); }
        const std::shared_ptr<Material>& AttachedMaterial() const { return m_pMaterial; }

        virtual void Draw(ShaderProgram* pShader);
        virtual void Draw();

        SquareMatrix4x4<float> ModelMatrix() const;
        SquareMatrix3x3<float> NormalMatrix() const;

        void             Color(const RGB_Color& val) { m_Color = val; }
        const RGB_Color& Color() const { return m_Color; }

        const Vector3<float>& Origin() const { return m_pPosition->Position(); }

        const Vector2<float>& Size() const { return m_Size; }
        void                  Size(const Vector2<float>& val) { m_Size = val; }

        void                 Rotation(const Radian<float>& val) { m_Rotation = val; }
        const Radian<float>& Rotation() const { return m_Rotation; }

        void Translate(const Vector3<float>& val) { m_Translation += val; }
        void Translation(Vector3<float> val) { m_Translation = std::move(val); }
        void Rotate(const Radian<float>& val) { m_Rotation += val; }

        void SizeX(float val) { m_Size[0] = val; }
        void SizeY(float val) { m_Size[1] = val; }

        void TranslateX(float val) { m_Translation.x(m_Translation.x() + val); }
        void TranslateY(float val) { m_Translation.y(m_Translation.y() + val); }
        void TranslateZ(float val) { m_Translation.z(m_Translation.z() + val); }
    };
} // namespace Ignosi::Modules::Render
