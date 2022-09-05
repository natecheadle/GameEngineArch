#pragma once

#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"

#include <DebugMutex.hpp>
#include <IWindow.h>

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

        std::mutex                                  m_ObjectQueue0_Mutex;
        std::queue<std::shared_ptr<const Object3D>> m_ObjectQueue0;
        std::mutex                                  m_Camera0Mutex;
        std::shared_ptr<const Camera3D>             m_pCamera0;

        std::mutex                                  m_ObjectQueue1_Mutex;
        std::queue<std::shared_ptr<const Object3D>> m_ObjectQueue1;
        std::mutex                                  m_Camera1Mutex;
        std::shared_ptr<const Camera3D>             m_pCamera1;

        std::mutex              m_PostEvent_Mutex;
        std::mutex              m_StartRunningMutex;
        std::condition_variable m_StartRunningCondition;

        DebugMutex m_IDMutex;
        int        m_PushingQueue_ID;

        GUI::IWindow*         m_pWindow;
        // TODO this shouldn't be here
        std::filesystem::path m_ShaderDir;

        std::atomic<bool> m_IsShutdown;
        std::atomic<bool> m_IsInitialized;

      public:
        // TODO this shouldn't need the shader dir
        RendererSM(GUI::IWindow* pWindow, std::filesystem::path shaderDir);
        ~RendererSM() = default;

        void StartRunning();

        void SetCamera(std::shared_ptr<const Camera3D> pCamera);

        void PushObjects(std::span<std::shared_ptr<const Object3D>> objects);
        void PushObject(std::shared_ptr<const Object3D> object);

        GUI::IWindow*                Window() const { return m_pWindow; }
        const std::filesystem::path& ShaderDir() const { return m_ShaderDir; }

        void PostEvent(const sc::event_base& ev);

        bool IsInitialized() const { return m_IsInitialized; }
        bool IsShutdown() const { return m_IsShutdown; }

      private:
        std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>>              GetCamera();
        std::pair<std::queue<std::shared_ptr<const Object3D>>&, std::unique_lock<std::mutex>> GetQueue();
        void                         ShutdownComplete() { m_IsShutdown = true; }
        void                         InitializationComplete() { m_IsInitialized = true; }
        std::unique_lock<std::mutex> AcquireStartRunningLock();
        void                         WaitStartRunning(std::unique_lock<std::mutex>& mtx);

        friend class RendererSM_Shutdown;
        friend class RendererSM_Processing_SubmittingObjects;
        friend class RendererSM_Processing_ConfiguringView;
        friend class RendererSM_Initialized;
    };
} // namespace nate::Modules::Render

#include "States/RendererSM_Initializing.h"