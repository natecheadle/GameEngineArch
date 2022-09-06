#pragma once

#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"

#include <DebugMutex.hpp>
#include <IWindow.h>
#include <bgfx/bgfx.h>

#include <boost/statechart/event_base.hpp>
#include <boost/statechart/state_machine.hpp>

#include <condition_variable>
#include <filesystem>
#include <memory>
#include <mutex>
#include <queue>
#include <span>
#include <vector>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Initializing;

    class RendererSM : public sc::state_machine<RendererSM, RendererSM_Initializing>
    {
        struct ShaderContext
        {
            const std::vector<std::uint8_t>& Data;
            std::mutex                       DataMutex;
            std::condition_variable          DataCondition;
            bgfx::ShaderHandle               Handle;
        };

        struct ProgramContext
        {
            bgfx::ShaderHandle      FragmentShader;
            bgfx::ShaderHandle      VertexShader;
            std::mutex              DataMutex;
            std::condition_variable DataCondition;
            bgfx::ProgramHandle     Handle;
        };

        std::mutex                                  m_ObjectQueue0_Mutex;
        std::queue<std::shared_ptr<const Object3D>> m_ObjectQueue0;
        std::mutex                                  m_Camera0Mutex;
        std::shared_ptr<const Camera3D>             m_pCamera0;

        std::mutex                                  m_ObjectQueue1_Mutex;
        std::queue<std::shared_ptr<const Object3D>> m_ObjectQueue1;
        std::mutex                                  m_Camera1Mutex;
        std::shared_ptr<const Camera3D>             m_pCamera1;

        std::mutex                  m_ShaderQueue_Mutex;
        std::queue<ShaderContext*>  m_CreateShaderQueue;
        std::mutex                  m_ProgramQueue_Mutex;
        std::queue<ProgramContext*> m_ProgramQueue;

        std::mutex m_PostEvent_Mutex;

        std::mutex m_IDMutex;
        int        m_PushingQueue_ID;

        GUI::IWindow* m_pWindow;

        std::atomic<bool> m_IsShutdown;
        std::atomic<bool> m_IsInitialized;
        std::atomic<bool> m_StartRunning;

      public:
        RendererSM(GUI::IWindow* pWindow);
        ~RendererSM() = default;

        void StartRunning() { m_StartRunning = true; }

        void SetCamera(std::shared_ptr<const Camera3D> pCamera);

        void PushObjects(std::span<std::shared_ptr<const Object3D>> objects);
        void PushObject(std::shared_ptr<const Object3D> object);

        GUI::IWindow* Window() const { return m_pWindow; }

        void PostEvent(const sc::event_base& ev);

        bool IsInitialized() const { return m_IsInitialized; }
        bool IsShutdown() const { return m_IsShutdown; }
        bool ShouldStart() const { return m_StartRunning; }

        bgfx::ShaderHandle  CreateShader(const std::vector<std::uint8_t>& data);
        bgfx::ProgramHandle CreateProgram(bgfx::ShaderHandle fragment, bgfx::ShaderHandle vertex);

      private:
        std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>>              GetCamera();
        std::pair<std::queue<std::shared_ptr<const Object3D>>&, std::unique_lock<std::mutex>> GetQueue();
        void ShutdownComplete() { m_IsShutdown = true; }
        void InitializationComplete() { m_IsInitialized = true; }
        void IncrementFrame();

        ShaderContext*  PopShaderContext();
        ProgramContext* PopProgramContext();

        static bgfx::ShaderHandle  GetShaderHandle(const std::vector<std::uint8_t>& data);
        static bgfx::ProgramHandle GetProgramHandle(bgfx::ShaderHandle fragment, bgfx::ShaderHandle vertex);

        friend class RendererSM_Shutdown;
        friend class RendererSM_Processing_SubmittingObjects;
        friend class RendererSM_Processing_ConfiguringView;
        friend class RendererSM_Initialized;
        friend class RendererSM_Processing_WaitingForFrame;
    };
} // namespace nate::Modules::Render

#include "States/RendererSM_Initializing.h"