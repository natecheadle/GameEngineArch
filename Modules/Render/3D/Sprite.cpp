#include "Sprite.h"

#include <3D/SpriteVertexData.h>
#include <LinearAlgebra/Vector.hpp>
#include <Renderer/Renderer.h>
#include <Units/Radian.hpp>

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

    Sprite::Sprite(Renderer* pRenderer)
        : m_pRenderer(pRenderer)
        , m_pBuffer(pRenderer->CreateBuffer(
              SpriteVertexData::describe(),
              std::span<const float>(
                  reinterpret_cast<const float*>(m_BasicSpritePoints),
                  reinterpret_cast<const float*>(m_BasicSpritePoints) + sizeof(m_BasicSpritePoints) / (sizeof(float)))))
    {
    }

    Sprite::Sprite(
        Renderer*                pRenderer,
        const VertexDataConfig&  config,
        std::span<float>         vertexes,
        std::span<std::uint32_t> indeces)
        : m_pRenderer(pRenderer)
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes, indeces))
    {
    }

    Sprite::Sprite(Renderer* pRenderer, const VertexDataConfig& config, std::span<const float> vertexes)
        : m_pRenderer(pRenderer)
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes))
    {
    }
    Sprite::Sprite(const Sprite& other)
        : m_pRenderer(other.m_pRenderer)
        , m_Origin(other.m_Origin)
        , m_Size(other.m_Size)
        , m_Rotation(other.m_Rotation)
        , m_pBuffer(other.m_pBuffer)
        , m_pMaterial(other.m_pMaterial)
        , m_Color(other.m_Color)
    {
    }
    SquareMatrix4x4<float> Sprite::ModelMatrix() const
    {
        if (m_Rotation == Radian<float>(0.0) && m_Origin == Vector2<float>({0.0f, 0.0f}) &&
            m_Size == Vector2<float>({1.0f, 1.0f}))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt{
            SquareMatrix4x4<float>::translate_init(Vector3<float>(m_Origin[0], m_Origin[1], 0.0))};
        rslt *= SquareMatrix4x4<float>::translate_init(Vector3<float>(m_Size[0] * 0.5f, m_Size[1] * 0.5f, 0.0));
        rslt *= SquareMatrix4x4<float>::rotate_z_init(m_Rotation);
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

    void Sprite::Draw(ShaderProgram* pShader)
    {
        pShader->SetShaderVar("model", ModelMatrix());
        pShader->SetShaderVar("spriteColor", Color().Data());

        Draw();
    }

    void Sprite::Draw()
    {
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

        m_pBuffer->Draw();
    }
} // namespace Ignosi::Modules::Render
