#pragma once

#include "3D/Material.h"
#include "3D/RGB_Color.h"
#include "IComponent.h"
#include "KinematicData.h"
#include "LinearAlgebra/Vector3.hpp"
#include "Renderer/VertexBuffer.h"
#include "Shader/ShaderProgram.h"
#include "SpriteVertexData.h"
#include "WeakComponentPointer.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector.hpp>
#include <Units/Radian.hpp>

#include <memory>
#include <span>

namespace Ignosi::Modules::Render
{
    class Renderer;

    class Sprite : ECS::IComponent
    {

        Renderer*                                         m_pRenderer{nullptr};
        ECS::WeakComponentPointer<Physics::KinematicData> m_pPosition;
        Vector2<float>                                    m_Size{1.0, 0.0};
        std::shared_ptr<VertexBuffer>                     m_pBuffer;
        std::shared_ptr<ShaderProgram>                    m_pShader{nullptr};
        std::shared_ptr<Material>                         m_pMaterial;
        RGB_Color                                         m_Color{1.0, 1.0, 1.0};

        static constexpr size_t                                             s_SpritePointsSize = 6U;
        static const std::array<const SpriteVertexData, s_SpritePointsSize> s_BasicSpritePoints;
        static const size_t                                                 s_SpritePointsFloatSize;

      public:
        Sprite() = default;
        Sprite(Renderer* pRenderer, ECS::WeakComponentPointer<Physics::KinematicData> pPosition, float aspectRatio = 1.0);
        Sprite(
            Renderer*                                         pRenderer,
            ECS::WeakComponentPointer<Physics::KinematicData> pPosition,
            const VertexDataConfig&                           config,
            std::span<float>                                  vertexes,
            std::span<std::uint32_t>                          indeces);
        Sprite(
            Renderer*                                         pRenderer,
            ECS::WeakComponentPointer<Physics::KinematicData> pPosition,
            const VertexDataConfig&                           config,
            std::span<const float>                            vertexes);

        virtual ~Sprite() = default;

        Sprite(const Sprite& other) = default;
        Sprite(Sprite&& other)      = default;

        Sprite& operator=(const Sprite& other)     = default;
        Sprite& operator=(Sprite&& other) noexcept = default;

        const ECS::Tag& Tag() const override
        {
            static const ECS::Tag s_tag = ECS::Tag::Create("Sprite");
            return s_tag;
        }
        void                                  Shader(std::shared_ptr<ShaderProgram> pProgram) { m_pShader = std::move(pProgram); }
        const std::shared_ptr<ShaderProgram>& Shader() const { return m_pShader; }

        void                             AttachedMaterial(std::shared_ptr<Material> pMat) { m_pMaterial = std::move(pMat); }
        const std::shared_ptr<Material>& AttachedMaterial() const { return m_pMaterial; }

        virtual void Draw();

        SquareMatrix4x4<float> ModelMatrix() const;
        SquareMatrix3x3<float> NormalMatrix() const;

        void             Color(const RGB_Color& val) { m_Color = val; }
        const RGB_Color& Color() const { return m_Color; }

        const Vector3<float>&         Origin() const { return m_pPosition->Position(); }
        const Vector3<Radian<float>>& Rotation() { return m_pPosition->Angle(); }

        const Vector2<float>& Size() const { return m_Size; }
        void                  Size(const Vector2<float>& val) { m_Size = val; }

        void SizeX(float val) { m_Size[0] = val; }
        void SizeY(float val) { m_Size[1] = val; }
    };
} // namespace Ignosi::Modules::Render
