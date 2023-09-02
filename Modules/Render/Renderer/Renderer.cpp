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
        Start();
    }

    Renderer::~Renderer()
    {
        Stop();
        Join();
    }

    void Renderer::DrawAllMesh(ShaderProgram* pProgram)
    { /*
         ExecuteFunction([&]() -> void {
             auto& pool = GetPool<Mesh3D>();
             for (auto& val : pool)
             {
                 val.Draw(pProgram);
             }
         });*/
    }

    void Renderer::DrawAllSprites(ShaderProgram* pProgram)
    { /*
         ExecuteFunction([&]() -> void {
             auto& pool = GetPool<Sprite>();
             for (auto& val : pool)
             {
                 val.Draw(pProgram);
             }
         });*/
    }

    void Renderer::ExecuteJob()
    {

        m_RenderThreadID = std::this_thread::get_id();
        while (!ShouldStop())
        {
            if (IsPaused())
            {
                Yield();
            }

            std::pair<std::optional<std::promise<void>>, std::optional<std::function<void()>>> pair = PopFunc();
            if (!pair.first.has_value())
            {
                continue;
            }

            assert(pair.first.has_value() && pair.second.has_value());
            auto& func = pair.second.value();
            auto& prom = pair.first.value();

            ExecuteFunction(prom, func);
        }
        FlushQueue();
    }

    void Renderer::ExecuteFunction(std::function<void()> func)
    {
        // Execution is already on the render thread context, so simply execute the operation.
        if (std::this_thread::get_id() == m_RenderThreadID)
        {
            func();
            return;
        }

        if (ShouldStop())
            return;

        std::future<void> fut = ExecuteFunctionAsync(std::move(func));
        return fut.get();
    }
    std::future<void> Renderer::ExecuteFunctionAsync(std::function<void()> func)
    {
        std::promise<void> prom;
        std::future<void>  fut = prom.get_future();
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_CommandQueue.push({std::move(prom), std::move(func)});
        }

        m_QueueCondition.notify_all();
        return std::move(fut);
    }

    std::pair<std::optional<std::promise<void>>, std::optional<std::function<void()>>> Renderer::PopFunc()
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);

        if (m_CommandQueue.empty())
        {
            m_QueueCondition.wait_for(lock, std::chrono::milliseconds(10));
            return {};
        }

        if (m_CommandQueue.empty())
        {
            m_QueueCondition.wait_for(lock, std::chrono::milliseconds(10));
        }

        std::pair<std::promise<void>, std::function<void()>> func = std::move(m_CommandQueue.front());
        m_CommandQueue.pop();
        return {std::move(func.first), std::move(func.second)};
    }

    void Renderer::ExecuteFunction(std::promise<void>& prom, std::function<void()>& func)
    {
        try
        {
            func();
            prom.set_value();
        }
        catch (...)
        {
            try
            {
                prom.set_exception(std::current_exception());
            }
            catch (...) // set_exception() may throw too
            {
            }
        }
    }

    void Renderer::FlushQueue()
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        while (!m_CommandQueue.empty())
        {
            std::pair<std::promise<void>, std::function<void()>> func = std::move(m_CommandQueue.front());
            ExecuteFunction(func.first, func.second);
        }
    }
} // namespace Ignosi::Modules::Render
