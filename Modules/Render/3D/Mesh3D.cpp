#include "Mesh3D.h"

#include "KinematicData.h"
#include "Renderer/Renderer.h"
#include "Shader/ShaderProgram.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

namespace Ignosi::Modules::Render
{
    const VertexData Mesh3D::m_CubePoints[] = {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {{0.0f, 0.0f}}},
        {{0.5f, -0.5f, -0.5f},  {0.0f, 0.0f, -1.0f}, {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, -0.5f},   {0.0f, 0.0f, -1.0f}, {{1.0f, 1.0f}}},
        {{0.5f, 0.5f, -0.5f},   {0.0f, 0.0f, -1.0f}, {{1.0f, 1.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {0.0f, 0.0f, -1.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {{0.0f, 0.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f},  {{0.0f, 0.0f}}},
        {{0.5f, -0.5f, 0.5f},   {0.0f, 0.0f, 1.0f},  {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {0.0f, 0.0f, 1.0f},  {{1.0f, 1.0f}}},
        {{0.5f, 0.5f, 0.5f},    {0.0f, 0.0f, 1.0f},  {{1.0f, 1.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {0.0f, 0.0f, 1.0f},  {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f},  {{0.0f, 0.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {-1.0f, 0.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {-1.0f, 0.0f, 0.0f}, {{1.0f, 1.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {-1.0f, 0.0f, 0.0f}, {{0.0f, 0.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {-1.0f, 0.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {1.0f, 0.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, -0.5f},   {1.0f, 0.0f, 0.0f},  {{1.0f, 1.0f}}},
        {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 0.0f},  {{0.0f, 1.0f}}},
        {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 0.0f},  {{0.0f, 1.0f}}},
        {{0.5f, -0.5f, 0.5f},   {1.0f, 0.0f, 0.0f},  {{0.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {1.0f, 0.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {{1.0f, 1.0f}}},
        {{0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {0.0f, -1.0f, 0.0f}, {{0.0f, 0.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f},  {{0.0f, 1.0f}}},
        {{0.5f, 0.5f, -0.5f},   {0.0f, 1.0f, 0.0f},  {{1.0f, 1.0f}}},
        {{0.5f, 0.5f, 0.5f},    {0.0f, 1.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {0.0f, 1.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {0.0f, 1.0f, 0.0f},  {{0.0f, 0.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f},  {{0.0f, 1.0f}}}
    };

    Mesh3D::Mesh3D(
        Renderer*                                         pRenderer,
        ECS::WeakComponentPointer<Physics::KinematicData> pPosition,
        const VertexDataConfig&                           config,
        std::span<const float>                            vertexes,
        std::span<const std::uint32_t>                    indeces)
        : m_pRenderer(pRenderer)
        , m_pPosition(std::move(pPosition))
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes, indeces))
    {
    }

    Mesh3D::Mesh3D(
        Renderer*                                         pRenderer,
        ECS::WeakComponentPointer<Physics::KinematicData> pPosition,
        const VertexDataConfig&                           config,
        std::span<const float>                            vertexes)
        : m_pRenderer(pRenderer)
        , m_pPosition(std::move(pPosition))
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes))
    {
    }

    Mesh3D Mesh3D::CreateCube(Renderer* pRenderer, ECS::WeakComponentPointer<Physics::KinematicData> pPosition)
    {
        return Mesh3D(
            pRenderer,
            std::move(pPosition),
            VertexData::describe(),
            std::span<const float>(
                reinterpret_cast<const float*>(m_CubePoints),
                reinterpret_cast<const float*>(m_CubePoints) + sizeof(m_CubePoints) / (sizeof(float))));
    }

    SquareMatrix4x4<float> Mesh3D::ModelMatrix() const
    {
        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::translate_init(m_pPosition->Position()));
        rslt *= SquareMatrix4x4<float>::rotate_zyx_init(m_pPosition->Angle());
        return rslt;
    }

    SquareMatrix3x3<float> Mesh3D::NormalMatrix() const
    {
        auto norm = ModelMatrix();
        norm.invert_this();
        norm.transpose_this();
        return norm.to_3x3();
    }

    void Mesh3D::Draw()
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
