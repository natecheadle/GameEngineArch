#include "Object3D.h"

#include "../Renderer/Renderer.h"
#include "SquareMatrix4x4.hpp"
#include "Vector3.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>

namespace nate::Modules::Render
{
    VertexData Object3D::m_CubePoints[] = {
        {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 0.0f}}},
        {{{0.5f, -0.5f, -0.5f}},  {}, {{1.0f, 0.0f}}},
        {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
        {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
        {{{-0.5f, 0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
        {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 0.0f}}},
        {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
        {{{0.5f, -0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
        {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 1.0f}}},
        {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 1.0f}}},
        {{{-0.5f, 0.5f, 0.5f}},   {}, {{0.0f, 1.0f}}},
        {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
        {{{-0.5f, 0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
        {{{-0.5f, 0.5f, -0.5f}},  {}, {{1.0f, 1.0f}}},
        {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
        {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
        {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
        {{{-0.5f, 0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
        {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
        {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
        {{{0.5f, -0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
        {{{0.5f, -0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
        {{{0.5f, -0.5f, 0.5f}},   {}, {{0.0f, 0.0f}}},
        {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
        {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
        {{{0.5f, -0.5f, -0.5f}},  {}, {{1.0f, 1.0f}}},
        {{{0.5f, -0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
        {{{0.5f, -0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
        {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
        {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
        {{{-0.5f, 0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
        {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
        {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
        {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
        {{{-0.5f, 0.5f, 0.5f}},   {}, {{0.0f, 0.0f}}},
        {{{-0.5f, 0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}}
    };

    Object3D::Object3D(Renderer* pRenderer, std::span<VertexData> vertexes, std::span<std::uint32_t> indeces)
        : m_pBuffer(pRenderer->CreateBuffer(vertexes, indeces))
    {
    }

    Object3D::Object3D(Renderer* pRenderer, std::span<VertexData> vertexes)
        : m_pBuffer(pRenderer->CreateBuffer(vertexes))
    {
    }

    std::unique_ptr<Object3D> Object3D::CreateCube(Renderer* pRenderer)
    {
        return std::make_unique<Object3D>(pRenderer, m_CubePoints);
    }

    SquareMatrix4x4<float> Object3D::ModelMatrix() const
    {
        if (m_Rotation == Vector3<Radian<float>>({0.0, 0.0, 0.0}) && m_Origin == Vector3<float>({0.0, 0.0, 0.0}))
        {
            return SquareMatrix4x4<float>::identity<SquareMatrix4x4<float>>();
        }

        SquareMatrix4x4<float> rslt(SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation));
        rslt *= SquareMatrix4x4<float>::translate_init(m_Origin);
        return rslt;
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
