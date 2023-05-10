#include "Renderer.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <exception>
#include <mutex>
#include <optional>

namespace nate::Modules::Render
{
    std::unique_ptr<Renderer> Renderer::s_pInstance{nullptr};

    void Renderer::SetInstance(std::unique_ptr<Renderer> pRenderer)
    {
        if (!pRenderer->IsExecuting())
        {
            pRenderer->Start();
        }

        s_pInstance = std::move(pRenderer);
    }

    Renderer::~Renderer()
    {
        Stop();
        Join();
    }

    void Renderer::ExecuteJob()
    {
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
    }

    void Renderer::ExecuteFunction(std::function<void()> func)
    {
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
            m_QueueCondition.notify_all();
        }
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
} // namespace nate::Modules::Render
