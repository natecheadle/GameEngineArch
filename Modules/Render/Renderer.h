#pragma once

#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"
#include "CursorPosition.hpp"
#include "KeyPressedInfo.hpp"
#include "MouseClickedInfo.hpp"
#include "StateMachine/RendererSM.h"
#include "WindowSize.hpp"

#include <IWindow.h>
#include <Job.h>

#include <atomic>
#include <condition_variable>
#include <exception>
#include <filesystem>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace nate::Modules::Render
{
    class RendererSM;
    class Renderer : private Jobs::Job
    {
      public:
        enum class RenderResult
        {
            NoContext,
            Render,
            Timeout,
            Exiting,
        };

        virtual ~Renderer();

        void Initialize(GUI::IWindow* pWindow);
        bool IsInitialized() const;
        bool IsRunning() const { return IsExecuting(); }
        void StartRendering();

        void Shutdown();

        bool                  RenderingFailed() const { return IsFailed(); }
        const std::exception& GetFailure() const { return GetCaughtException(); }

        // Must be called during App::UpdateApp
        void AttachCamera(std::shared_ptr<const Camera3D> pCamera);
        // Must be called during App::UpdateApp
        void RenderObject(std::shared_ptr<const Object3D> pObject);

        RenderResult        RenderFrame();
        bgfx::ShaderHandle  CreateShader(const std::vector<std::uint8_t>& data);
        bgfx::ProgramHandle CreateProgram(bgfx::ShaderHandle fragment, bgfx::ShaderHandle vertex);

      protected:
        void ExecuteJob() final;

      private:
        void PrivShutdown();

        std::unique_ptr<RendererSM> m_pRendererSM;
    };
} // namespace nate::Modules::Render
