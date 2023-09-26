#include "Renderer.h"

#include "3D/Sprite.h"
#include "ComponentPool.h"
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

    void Renderer::Update(double dt)
    {
        ClearColorBuffer();
        ClearDepthBuffer();

        m_pCamera->Update(std::chrono::nanoseconds((unsigned long long)(dt * 1e9)));
        auto& meshpool   = GetPool<Mesh3D>();
        auto& spritepool = GetPool<Sprite>();

        const auto& meshEntities   = World()->GetEntitiesByTag(ECS::ComponentPool<Mesh3D>::ComponentTag());
        const auto& spriteEntities = World()->GetEntitiesByTag(ECS::ComponentPool<Sprite>::ComponentTag());
        for (const auto& entity : meshEntities)
        {
            auto&       mesh    = meshpool.GetComponent(World()->GetEntity(entity));
            const auto& pShader = mesh.Shader();

            pShader->Use();
            pShader->SetShaderVar("view", m_pCamera->ViewPerspective());
            pShader->SetShaderVar("viewPos", m_pCamera->CameraPosition());
            pShader->SetShaderVar("projection", m_pCamera->Projection());

            mesh.Draw();
        }
        for (const auto& entity : spriteEntities)
        {
            auto& sprite = spritepool.GetComponent(World()->GetEntity(entity));

            const auto& pShader = sprite.Shader();
            pShader->Use();
            pShader->SetShaderVar("projection", m_pCamera->ViewOrthographic());
            sprite.Draw();
        }

        SwapBuffers();
    }

} // namespace Ignosi::Modules::Render
