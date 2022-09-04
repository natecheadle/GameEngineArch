#pragma once

#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"

#include <DebugMutex.hpp>
#include <IWindow.h>

#include <boost/statechart/event_base.hpp>
#include <boost/statechart/state_machine.hpp>

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

        std::mutex                                  ObjectQueue0_Mutex;
        std::queue<std::shared_ptr<const Object3D>> ObjectQueue0;
        std::mutex                                  Camera0Mutex;
        std::shared_ptr<const Camera3D>             pCamera0;

        std::mutex                                  ObjectQueue1_Mutex;
        std::queue<std::shared_ptr<const Object3D>> ObjectQueue1;
        std::mutex                                  Camera1Mutex;
        std::shared_ptr<const Camera3D>             pCamera1;

        std::mutex PostEvent_Mutex;

        DebugMutex m_IDMutex;
        int        PushingQueue_ID;

        GUI::IWindow*         m_pWindow;
        // TODO this shouldn't be here
        std::filesystem::path m_ShaderDir;

      public:
        // TODO this shouldn't need the shader dir
        RendererSM(GUI::IWindow* pWindow, std::filesystem::path shaderDir);

        void SetCamera(std::shared_ptr<const Camera3D> pCamera);
        std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>> GetCamera();

        void PushObjects(std::span<std::shared_ptr<const Object3D>> objects);
        void PushObject(std::shared_ptr<const Object3D> object);

        std::pair<std::queue<std::shared_ptr<const Object3D>>&, std::unique_lock<std::mutex>> GetQueue();

        GUI::IWindow*                Window() const { return m_pWindow; }
        const std::filesystem::path& ShaderDir() const { return m_ShaderDir; }

        void PostEvent(const sc::event_base& ev);
    };
} // namespace nate::Modules::Render

#include "States/RendererSM_Initializing.h"