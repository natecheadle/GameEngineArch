#include "RendererSM.h"

#include "DebugMutex.hpp"

#include <mutex>

namespace nate::Modules::Render
{
    RendererSM::RendererSM(GUI::IWindow* pWindow, std::filesystem::path shaderDir)
        : m_pWindow(pWindow)
        , m_ShaderDir(std::move(shaderDir))
    {
    }

    void RendererSM::SetCamera(std::shared_ptr<const Camera3D> pCamera)
    {
        std::unique_lock<DebugMutex> lock(m_IDMutex);
        if (m_PushingQueue_ID == 0)
        {
            std::unique_lock<std::mutex> camLock(m_Camera0Mutex);
            m_pCamera0 = std::move(pCamera);
        }
        else
        {
            std::unique_lock<std::mutex> camLock(m_Camera1Mutex);
            m_pCamera1 = std::move(pCamera);
        }
    }

    std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>> RendererSM::GetCamera()
    {
        std::unique_lock<DebugMutex> lock(m_IDMutex);
        if (m_PushingQueue_ID == 0)
        {
            std::unique_lock<std::mutex> camLock(m_Camera0Mutex);
            return std::move(std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>>(
                m_pCamera0,
                std::move(camLock)));
        }
        std::unique_lock<std::mutex> camLock(m_Camera1Mutex);
        return std::move(
            std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>>(m_pCamera1, std::move(camLock)));
    }

    void RendererSM::PushObjects(std::span<std::shared_ptr<const Object3D>> objects)
    {
        std::unique_lock<DebugMutex> lock(m_IDMutex);
        if (m_PushingQueue_ID == 0)
        {
            std::unique_lock<std::mutex> queue_lock(m_ObjectQueue0_Mutex);
            for (auto& object : objects)
            {
                m_ObjectQueue0.push(std::move(object));
            }
        }
        else
        {
            std::unique_lock<std::mutex> queue_lock(m_ObjectQueue1_Mutex);
            for (auto& object : objects)
            {
                m_ObjectQueue1.push(std::move(object));
            }
        }
    }

    void RendererSM::PushObject(std::shared_ptr<const Object3D> object)
    {
        std::unique_lock<DebugMutex> lock(m_IDMutex);
        if (m_PushingQueue_ID == 0)
        {
            std::unique_lock<std::mutex> queue_lock(m_ObjectQueue0_Mutex);
            m_ObjectQueue0.push(std::move(object));
        }
        else
        {
            std::unique_lock<std::mutex> queue_lock(m_ObjectQueue1_Mutex);
            m_ObjectQueue1.push(std::move(object));
        }
    }

    std::pair<std::queue<std::shared_ptr<const Object3D>>&, std::unique_lock<std::mutex>> RendererSM::GetQueue()
    {
        std::unique_lock<DebugMutex> lock(m_IDMutex);
        if (m_PushingQueue_ID == 0)
        {
            std::unique_lock<std::mutex> queue_lock(m_ObjectQueue0_Mutex);
            return std::move(std::pair<std::queue<std::shared_ptr<const Object3D>>&, std::unique_lock<std::mutex>>(
                m_ObjectQueue0,
                std::move(queue_lock)));
        }

        std::unique_lock<std::mutex> queue_lock(m_ObjectQueue1_Mutex);
        return std::move(std::pair<std::queue<std::shared_ptr<const Object3D>>&, std::unique_lock<std::mutex>>(
            m_ObjectQueue1,
            std::move(queue_lock)));
    }

    void RendererSM::PostEvent(const sc::event_base& ev)
    {
        std::unique_lock<std::mutex> lock(m_PostEvent_Mutex);
        post_event(ev);
    }

    std::unique_lock<std::mutex> RendererSM::AcquireStartRunningLock()
    {
        return std::move(std::unique_lock<std::mutex>(m_StartRunningMutex));
    }

    void RendererSM::WaitStartRunning(std::unique_lock<std::mutex>& mtx)
    {
        m_StartRunningCondition.wait(mtx);
    }

    void RendererSM::StartRunning()
    {
        assert(m_IsInitialized);
        m_StartRunningCondition.notify_one();
    }
} // namespace nate::Modules::Render