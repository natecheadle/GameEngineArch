#include "RendererSM.h"

#include "DebugMutex.hpp"

#include <cstddef>
#include <mutex>

namespace nate::Modules::Render
{
    RendererSM::RendererSM(GUI::IWindow* pWindow)
        : m_pWindow(pWindow)
        , m_IsShutdown(false)
        , m_IsInitialized(false)
        , m_StartRunning(false)
    {
    }

    void RendererSM::SetCamera(std::shared_ptr<const Camera3D> pCamera)
    {
        std::unique_lock<std::mutex> lock(m_IDMutex);
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
        std::unique_lock<std::mutex> lock(m_IDMutex);
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
        std::unique_lock<std::mutex> lock(m_IDMutex);
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
        std::unique_lock<std::mutex> lock(m_IDMutex);
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
        std::unique_lock<std::mutex> lock(m_IDMutex);
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

    bgfx::ShaderHandle RendererSM::CreateShader(const std::vector<std::uint8_t>& data)
    {
        ShaderContext                context{.Data = data};
        std::unique_lock<std::mutex> dataLock(context.DataMutex);
        {
            std::unique_lock<std::mutex> lock(m_ShaderQueue_Mutex);
            m_CreateShaderQueue.push(&context);
        }
        context.DataCondition.wait(dataLock);
        return context.Handle;
    }

    bgfx::ProgramHandle RendererSM::CreateProgram(bgfx::ShaderHandle fragment, bgfx::ShaderHandle vertex)
    {
        ProgramContext               context{.FragmentShader = fragment, .VertexShader = vertex};
        std::unique_lock<std::mutex> dataLock(context.DataMutex);
        {
            std::unique_lock<std::mutex> lock(m_ProgramQueue_Mutex);
            m_ProgramQueue.push(&context);
        }
        context.DataCondition.wait(dataLock);
        return context.Handle;
    }

    bgfx::ShaderHandle RendererSM::GetShaderHandle(const std::vector<std::uint8_t>& data)
    {
        assert(data.size() <= (size_t)std::numeric_limits<uint32_t>::max());
        const bgfx::Memory* mem = bgfx::makeRef(data.data(), (uint32_t)data.size());
        return bgfx::createShader(mem);
    }

    bgfx::ProgramHandle RendererSM::GetProgramHandle(bgfx::ShaderHandle fragment, bgfx::ShaderHandle vertex)
    {
        return bgfx::createProgram(fragment, vertex, false);
    }

    RendererSM::ShaderContext* RendererSM::PopShaderContext()
    {
        std::unique_lock<std::mutex> lock(m_ShaderQueue_Mutex);
        if (m_CreateShaderQueue.empty())
            return nullptr;

        ShaderContext* pReturn = m_CreateShaderQueue.front();
        m_CreateShaderQueue.pop();
        return pReturn;
    }

    RendererSM::ProgramContext* RendererSM::PopProgramContext()
    {
        std::unique_lock<std::mutex> lock(m_ProgramQueue_Mutex);
        if (m_ProgramQueue.empty())
            return nullptr;

        ProgramContext* pReturn = m_ProgramQueue.front();
        m_ProgramQueue.pop();
        return pReturn;
    }

    void RendererSM::IncrementFrame()
    {
        std::unique_lock<std::mutex> lock(m_IDMutex);
        m_PushingQueue_ID = (m_PushingQueue_ID + 1) % 2;
    }
} // namespace nate::Modules::Render