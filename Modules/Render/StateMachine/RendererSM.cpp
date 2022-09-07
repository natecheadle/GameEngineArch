#include "RendererSM.h"

#include "DebugMutex.hpp"

#include <cstddef>
#include <mutex>
#include <thread>

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
        m_pCamera = std::move(pCamera);
    }

    std::shared_ptr<const Camera3D> RendererSM::GetCamera()
    {
        return m_pCamera;
    }

    void RendererSM::PushObjects(std::span<std::shared_ptr<const Object3D>> objects)
    {
        for (auto& object : objects)
        {
            m_ObjectQueue.push(std::move(object));
        }
    }

    void RendererSM::PushObject(std::shared_ptr<const Object3D> object)
    {
        m_ObjectQueue.push(std::move(object));
    }

    std::queue<std::shared_ptr<const Object3D>>& RendererSM::GetQueue()
    {
        return m_ObjectQueue;
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

    void RendererSM::WaitSubmission()
    {
        std::unique_lock<std::mutex> lock(m_SubmitObjects_Mutex);
        m_IsWiatingSubmission = true;
        m_SubmitObjects_Condition.wait(lock);
    }

    void RendererSM::SubmitObjects()
    {
        bool isWaiting = false;
        do
        {
            {
                std::unique_lock<std::mutex> lock(m_SubmitObjects_Mutex);
                isWaiting = m_IsWiatingSubmission;
            }
            if (!isWaiting)
            {
                std::this_thread::yield();
            }
        } while (!isWaiting);

        m_SubmitObjects_Condition.notify_all();
    }
} // namespace nate::Modules::Render