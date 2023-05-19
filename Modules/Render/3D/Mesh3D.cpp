#include "Mesh3D.h"

#include "../Renderer/Renderer.h"
#include "Shader/ShaderProgram.h"
#include "SquareMatrix4x4.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

namespace nate::Modules::Render
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
        Renderer*                      pRenderer,
        const VertexDataConfig&        config,
        std::span<const float>         vertexes,
        std::span<const std::uint32_t> indeces)
        : m_pRenderer(pRenderer)
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes, indeces))
    {
    }

    Mesh3D::Mesh3D(Renderer* pRenderer, const VertexDataConfig& config, std::span<const float> vertexes)
        : m_pRenderer(pRenderer)
        , m_pBuffer(pRenderer->CreateBuffer(config, vertexes))
    {
    }

    std::unique_ptr<Mesh3D> Mesh3D::CreateCube(Renderer* pRenderer)
    {
        return std::make_unique<Mesh3D>(
            pRenderer,
            VertexData::describe(),
            std::span<const float>(
                reinterpret_cast<const float*>(m_CubePoints),
                reinterpret_cast<const float*>(m_CubePoints) + sizeof(m_CubePoints) / (sizeof(float))));
    }

    SquareMatrix4x4<float> Mesh3D::ModelMatrix() const
    {
        if (m_Rotation == Vector3<Radian<float>>(0.0, 0.0, 0.0) && m_Origin == Vector3<float>(0.0, 0.0, 0.0))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::translate_init(m_Origin));
        rslt *= SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation);
        return rslt;
    }

    SquareMatrix3x3<float> Mesh3D::NormalMatrix() const
    {
        auto norm = ModelMatrix();
        norm.invert_this();
        norm.transpose_this();
        return norm.to_3x3();
    }

    void Mesh3D::Draw(ShaderProgram* pShader)
    {
        pShader->SetShaderVar("model", ModelMatrix());
        pShader->SetShaderVar("norm_mat", NormalMatrix());

        Draw();
    }

    void Mesh3D::Draw()
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
} // namespace nate::Modules::Render
