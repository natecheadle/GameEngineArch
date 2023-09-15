#include "Sprite.h"

#include "KinematicData.h"

#include <3D/SpriteVertexData.h>
#include <LinearAlgebra/Vector.hpp>
#include <Renderer/Renderer.h>
#include <Units/Radian.hpp>

#include <cassert>

namespace Ignosi::Modules::Render
{
    const std::array<const SpriteVertexData, Sprite::s_SpritePointsSize> Sprite::s_BasicSpritePoints = {
        {

         {{0.0f, 1.0f}, {0.0f, 1.0f}},
         {{1.0f, 0.0f}, {1.0f, 0.0f}},
         {{0.0f, 0.0f}, {0.0f, 0.0f}},

         {{0.0f, 1.0f}, {0.0f, 1.0f}},
         {{1.0f, 1.0f}, {1.0f, 1.0f}},
         {{1.0f, 0.0f}, {1.0f, 0.0f}},
         }
    };

    Sprite::Sprite(Renderer* pRenderer, ECS::WeakComponentPointer<Physics::KinematicData> pPosition, float aspectRatio)
        : m_pRenderer(pRenderer)
        , m_pPosition(std::move(pPosition))
    {

        if (aspectRatio != 1.0)
        {
            std::array<SpriteVertexData, s_SpritePointsSize> spritePoints;
            std::copy(s_BasicSpritePoints.begin(), s_BasicSpritePoints.end(), spritePoints.begin());
            if (aspectRatio > 1.0)
            {
                for (auto& point : spritePoints)
                {
                    point.Position[1] *= aspectRatio;
                }
            }
            else
            {
                float asp_inv = 1.0f / aspectRatio;
                for (auto& point : spritePoints)
                {
                    point.Position[0] *= asp_inv;
                }
            }

            m_pBuffer = pRenderer->CreateBuffer(
                SpriteVertexData::describe(),
                std::span<const float>(
                    reinterpret_cast<const float*>(spritePoints.data()),
                    reinterpret_cast<const float*>(spritePoints.data()) + s_SpritePointsFloatSize));
        }
        else
        {
            m_pBuffer = pRenderer->CreateBuffer(
                SpriteVertexData::describe(),
                std::span<const float>(
                    reinterpret_cast<const float*>(s_BasicSpritePoints.data()),
                    reinterpret_cast<const float*>(s_BasicSpritePoints.data()) + s_SpritePointsFloatSize));
        }
    }

    Sprite::Sprite(
        Renderer*                                         pRenderer,
        ECS::WeakComponentPointer<Physics::KinematicData> pPosition,
        const VertexDataConfig&                           config,
        std::span<float>                                  vertexes,
        std::span<std::uint32_t>                          indeces)
        : m_pRenderer(pRenderer)
        , m_pPosition(std::move(pPosition))
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes, indeces))
    {
    }

    Sprite::Sprite(
        Renderer*                                         pRenderer,
        ECS::WeakComponentPointer<Physics::KinematicData> pPosition,
        const VertexDataConfig&                           config,
        std::span<const float>                            vertexes)
        : m_pRenderer(pRenderer)
        , m_pPosition(std::move(pPosition))
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes))
    {
    }

    SquareMatrix4x4<float> Sprite::ModelMatrix() const
    {
        SquareMatrix4x4<float> rslt{SquareMatrix4x4<float>::translate_init(m_pPosition->Position())};
        rslt *= SquareMatrix4x4<float>::translate_init(Vector3<float>(m_Size[0] * 0.5f, m_Size[1] * 0.5f, 0.0));
        rslt *= SquareMatrix4x4<float>::rotate_z_init(m_pPosition->Angle().z());
        rslt *= SquareMatrix4x4<float>::translate_init(Vector3<float>(m_Size[0] * -0.5f, m_Size[1] * -0.5f, 0.0));
        rslt *= SquareMatrix4x4<float>::scale_init(Vector3<float>(m_Size[0], m_Size[1], 1.0));

        return rslt;
    }

    SquareMatrix3x3<float> Sprite::NormalMatrix() const
    {
        auto norm = ModelMatrix();
        norm.invert_this();
        norm.transpose_this();
        return norm.to_3x3();
    }

    void Sprite::Draw()
    {
        if (m_pShader)
        {
            m_pShader->SetShaderVar("model", ModelMatrix());
            m_pShader->SetShaderVar("norm_mat", NormalMatrix());
            if (m_pMaterial)
            {
                m_pShader->SetShaderVar("material", *m_pMaterial);
                auto activeTexture = [](const std::shared_ptr<Texture>& texs) {
                    if (texs)
                    {
                        texs->Activate();
                        texs->Bind();
                    }
                };
                activeTexture(m_pMaterial->Diffuse);
                activeTexture(m_pMaterial->Specular);
                activeTexture(m_pMaterial->Height);
                activeTexture(m_pMaterial->Normal);
            }
        }

        m_pBuffer->Draw();
        if (m_pMaterial->Diffuse)
            m_pMaterial->Diffuse->Unbind();
        if (m_pMaterial->Specular)
            m_pMaterial->Specular->Unbind();
        if (m_pMaterial->Height)
            m_pMaterial->Height->Unbind();
        if (m_pMaterial->Normal)
            m_pMaterial->Normal->Unbind();
    }
} // namespace Ignosi::Modules::Render
