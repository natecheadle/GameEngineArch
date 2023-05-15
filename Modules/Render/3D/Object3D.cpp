#include "Object3D.h"

#include "../Renderer/Renderer.h"
#include "Renderer/Color.h"
#include "Renderer/VertexData.h"
#include "SquareMatrix4x4.hpp"
#include "Vector3.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

namespace nate::Modules::Render
{
    VertexData Object3D::m_CubePoints[] = {
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f, -1.0f}, {{0.0f, 0.0f}}},
        {{0.5f, -0.5f, -0.5f},  {}, {0.0f, 0.0f, -1.0f}, {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, -0.5f},   {}, {0.0f, 0.0f, -1.0f}, {{1.0f, 1.0f}}},
        {{0.5f, 0.5f, -0.5f},   {}, {0.0f, 0.0f, -1.0f}, {{1.0f, 1.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {}, {0.0f, 0.0f, -1.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f, -1.0f}, {{0.0f, 0.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {}, {0.0f, 0.0f, 1.0f},  {{0.0f, 0.0f}}},
        {{0.5f, -0.5f, 0.5f},   {}, {0.0f, 0.0f, 1.0f},  {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {}, {0.0f, 0.0f, 1.0f},  {{1.0f, 1.0f}}},
        {{0.5f, 0.5f, 0.5f},    {}, {0.0f, 0.0f, 1.0f},  {{1.0f, 1.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {}, {0.0f, 0.0f, 1.0f},  {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {}, {0.0f, 0.0f, 1.0f},  {{0.0f, 0.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {}, {-1.0f, 0.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {}, {-1.0f, 0.0f, 0.0f}, {{1.0f, 1.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {}, {-1.0f, 0.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {}, {-1.0f, 0.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {}, {-1.0f, 0.0f, 0.0f}, {{0.0f, 0.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {}, {-1.0f, 0.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {}, {1.0f, 0.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, -0.5f},   {}, {1.0f, 0.0f, 0.0f},  {{1.0f, 1.0f}}},
        {{0.5f, -0.5f, -0.5f},  {}, {1.0f, 0.0f, 0.0f},  {{0.0f, 1.0f}}},
        {{0.5f, -0.5f, -0.5f},  {}, {1.0f, 0.0f, 0.0f},  {{0.0f, 1.0f}}},
        {{0.5f, -0.5f, 0.5f},   {}, {1.0f, 0.0f, 0.0f},  {{0.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {}, {1.0f, 0.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, -1.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{0.5f, -0.5f, -0.5f},  {}, {0.0f, -1.0f, 0.0f}, {{1.0f, 1.0f}}},
        {{0.5f, -0.5f, 0.5f},   {}, {0.0f, -1.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{0.5f, -0.5f, 0.5f},   {}, {0.0f, -1.0f, 0.0f}, {{1.0f, 0.0f}}},
        {{-0.5f, -0.5f, 0.5f},  {}, {0.0f, -1.0f, 0.0f}, {{0.0f, 0.0f}}},
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, -1.0f, 0.0f}, {{0.0f, 1.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {}, {0.0f, 1.0f, 0.0f},  {{0.0f, 1.0f}}},
        {{0.5f, 0.5f, -0.5f},   {}, {0.0f, 1.0f, 0.0f},  {{1.0f, 1.0f}}},
        {{0.5f, 0.5f, 0.5f},    {}, {0.0f, 1.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{0.5f, 0.5f, 0.5f},    {}, {0.0f, 1.0f, 0.0f},  {{1.0f, 0.0f}}},
        {{-0.5f, 0.5f, 0.5f},   {}, {0.0f, 1.0f, 0.0f},  {{0.0f, 0.0f}}},
        {{-0.5f, 0.5f, -0.5f},  {}, {0.0f, 1.0f, 0.0f},  {{0.0f, 1.0f}}}
    };

    Object3D::Object3D(
        Renderer*                pRenderer,
        const VertexDataConfig&  config,
        std::span<float>         vertexes,
        std::span<std::uint32_t> indeces)
        : m_pBuffer(pRenderer->CreateBuffer(config, vertexes, indeces))
    {
    }

    Object3D::Object3D(Renderer* pRenderer, const VertexDataConfig& config, std::span<float> vertexes)
        : m_pBuffer(pRenderer->CreateBuffer(config, vertexes))
    {
    }

    std::unique_ptr<Object3D> Object3D::CreateCube(Renderer* pRenderer)
    {
        return std::make_unique<Object3D>(
            pRenderer,
            VertexData::describe(),
            std::span<float>(
                reinterpret_cast<float*>(m_CubePoints),
                reinterpret_cast<float*>(m_CubePoints) + sizeof(m_CubePoints) / (sizeof(float))));
    }

    SquareMatrix4x4<float> Object3D::ModelMatrix() const
    {
        if (m_Rotation == Vector3<Radian<float>>(0.0, 0.0, 0.0) && m_Origin == Vector3<float>(0.0, 0.0, 0.0))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation));
        rslt *= SquareMatrix4x4<float>::translate_init(m_Origin);
        return rslt;
    }

    SquareMatrix3x3<float> Object3D::NormalMatrix() const
    {
        auto norm = ModelMatrix();
        norm.invert_this();
        norm.transpose_this();
        return norm.to_3x3();
    }

    void Object3D::Draw()
    {
        for (auto& tex : m_Textures)
        {
            if (tex)
            {
                tex->Activate();
                tex->Bind();
            }
        }
        m_pShader->Use();
        m_pBuffer->Draw();
    }
} // namespace nate::Modules::Render
