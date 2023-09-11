#include "Sprite.h"

#include "KinematicData.h"

#include <3D/SpriteVertexData.h>
#include <LinearAlgebra/Vector.hpp>
#include <Renderer/Renderer.h>
#include <Units/Radian.hpp>

#include <cassert>

namespace Ignosi::Modules::Render
{
    const SpriteVertexData Sprite::m_BasicSpritePoints[] = {

        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}},

        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}},
    };

    Sprite::Sprite(Renderer* pRenderer, Physics::KinematicData* pPosition)
        : m_pRenderer(pRenderer)
        , m_pPosition(pPosition)
        , m_pBuffer(pRenderer->CreateBuffer(
              SpriteVertexData::describe(),
              std::span<const float>(
                  reinterpret_cast<const float*>(m_BasicSpritePoints),
                  reinterpret_cast<const float*>(m_BasicSpritePoints) + sizeof(m_BasicSpritePoints) / (sizeof(float)))))
    {
        assert(m_pPosition);
    }

    Sprite::Sprite(
        Renderer*                pRenderer,
        Physics::KinematicData*  pPosition,
        const VertexDataConfig&  config,
        std::span<float>         vertexes,
        std::span<std::uint32_t> indeces)
        : m_pRenderer(pRenderer)
        , m_pPosition(pPosition)
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes, indeces))
    {
        assert(m_pPosition);
    }

    Sprite::Sprite(Renderer* pRenderer, Physics::KinematicData* pPosition, const VertexDataConfig& config, std::span<const float> vertexes)
        : m_pRenderer(pRenderer)
        , m_pPosition(pPosition)
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes))
    {
        assert(m_pPosition);
    }

    SquareMatrix4x4<float> Sprite::ModelMatrix() const
    {
        SquareMatrix4x4<float> rslt{SquareMatrix4x4<float>::translate_init(m_pPosition->Position())};
        rslt *= SquareMatrix4x4<float>::translate_init(Vector3<float>(m_Size[0] * 0.5f, m_Size[1] * 0.5f, 0.0));
        rslt *= SquareMatrix4x4<float>::rotate_z_init(m_pPosition->Angle().z());
        rslt *= SquareMatrix4x4<float>::translate_init(Vector3<float>(m_Size[0] * -0.5f, m_Size[1] * -0.5f, 0.0));
        rslt *= SquareMatrix4x4<float>::scale_init(Vector3<float>(m_Size[0], m_Size[1], 0.0));

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
    }
} // namespace Ignosi::Modules::Render
