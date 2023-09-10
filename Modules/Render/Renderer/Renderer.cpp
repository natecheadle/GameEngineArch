#include "Renderer.h"

#include "Renderer_OpenGL.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <exception>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>

namespace Ignosi::Modules::Render
{
    Renderer::Renderer(ECS::ComponentPool<Mesh3D>* pMeshPool, ECS::ComponentPool<Sprite>* pSpritePool)
        : ECS::System<Mesh3D, Sprite>(pMeshPool, pSpritePool)
    {
    }

    Renderer::~Renderer() {}

    void Renderer::DrawAllMesh(ShaderProgram* pProgram)
    {
        auto&       pool     = GetPool<Mesh3D>();
        const auto& entities = World()->GetEntitiesByTag(Tag());
        for (const auto& entity : entities)
        {
            auto& val = pool.GetComponent(World()->GetEntity(entity));
            val.Draw(pProgram);
        }
    }

    void Renderer::DrawAllSprites(ShaderProgram* pProgram)
    {
        auto&       pool     = GetPool<Sprite>();
        const auto& entities = World()->GetEntitiesByTag(Tag());
        for (const auto& entity : entities)
        {
            auto& val = pool.GetComponent(World()->GetEntity(entity));
            val.Draw(pProgram);
        }
    }

} // namespace Ignosi::Modules::Render
