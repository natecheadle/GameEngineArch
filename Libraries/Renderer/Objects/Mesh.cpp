#include "Mesh.h"

#include "IRenderer.h"

#include <span>

namespace Ignosi::Libraries::Renderer
{
    const VertexData Mesh::s_CubePoints[] = {
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

    const VertexData Mesh::s_SpritePoints[] = {

        {{0.5f, 0.5f, 0.0f},   {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

        {{0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
    };

    Mesh::Mesh(const IVertexBuffer* vertexes, const IShaderProgram* shader, const IMaterial* material)
        : m_Vertexes(vertexes)
        , m_Shader(shader)
        , m_Material(material)
    {
        assert(m_Vertexes);
        assert(m_Shader);
    }

    Math::SquareMatrix4x4<float> Mesh::ModelMatrix() const
    {
        Math::SquareMatrix4x4<float> rslt(Math::SquareMatrix4x4<float>::translate_init(m_Translation));
        rslt *= Math::SquareMatrix4x4<float>::rotate_zyx_init(m_Rotation);
        return rslt;
    }

    Math::SquareMatrix3x3<float> Mesh::NormalMatrix() const
    {
        auto norm = ModelMatrix();
        norm.invert_this();
        norm.transpose_this();
        return norm.to_3x3();
    }

    std::unique_ptr<IVertexBuffer> Mesh::CreateCubeVertexes(const IRenderer* pRenderer)
    {
        return pRenderer->CreateBuffer(
            VertexData::describe(),
            std::span<const float>(
                reinterpret_cast<const float*>(s_CubePoints),
                reinterpret_cast<const float*>(s_CubePoints) + sizeof(s_CubePoints) / (sizeof(float))));
    }

    std::unique_ptr<IVertexBuffer> Mesh::CreateSpriteVertexes(const IRenderer* pRenderer)
    {
        return pRenderer->CreateBuffer(
            VertexData::describe(),
            std::span<const float>(
                reinterpret_cast<const float*>(s_SpritePoints),
                reinterpret_cast<const float*>(s_SpritePoints) + sizeof(s_SpritePoints) / (sizeof(float))));
    }
} // namespace Ignosi::Libraries::Renderer
