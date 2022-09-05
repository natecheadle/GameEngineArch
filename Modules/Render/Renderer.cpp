#include "Renderer.h"

#include "CursorPosition.hpp"
#include "Keys.h"
#include "Matrix4x4.h"
#include "Shader/BGFX_Shader.h"
#include "StateMachine/Events/RendererSM_EV_ShutdownRequested.hpp"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <chrono>
#include <exception>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <thread>

namespace nate::Modules::Render
{
    void Renderer::Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc)
    {
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();
        m_pRendererSM = std::make_unique<RendererSM>(pWindow, std::move(shaderLoc));
        Start();
    }

    Renderer::~Renderer()
    {
        PrivShutdown();
    }

    void Renderer::Shutdown()
    {
        PrivShutdown();
    }

    void Renderer::AttachCamera(std::shared_ptr<const Camera3D> pCamera)
    {
        m_pRendererSM->SetCamera(std::move(pCamera));
    }

    void Renderer::RenderObject(std::shared_ptr<const Object3D> pObject)
    {
        m_pRendererSM->PushObject(std::move(pObject));
    }

    void Renderer::RenderFrame()
    {
        bgfx::renderFrame();
    }

    void Renderer::ExecuteJob()
    {
        m_pRendererSM->initiate();
    }

    void Renderer::PrivShutdown()
    {
        if (m_pRendererSM)
        {
            m_pRendererSM->PostEvent(RendererSM_EV_ShutdownRequested());
            bgfx::renderFrame();
            while (!m_pRendererSM->IsShutdown())
            {
                std::this_thread::yield();
            }

            m_pRendererSM->terminate();
            m_pRendererSM.reset();
        }
    }

    void Renderer::StartRendering()
    {
        assert(m_pRendererSM);

        while (!m_pRendererSM->IsInitialized())
        {
            std::this_thread::yield();
            bgfx::renderFrame();
        }

        m_pRendererSM->StartRunning();
    }
} // namespace nate::Modules::Render