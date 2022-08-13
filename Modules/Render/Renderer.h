#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"
#include "CursorPosition.hpp"
#include "IRenderer.h"
#include "KeyPressedInfo.hpp"
#include "MouseClickedInfo.hpp"
#include "WindowSize.hpp"

#include <IWindow.h>
#include <Job.h>

#include <atomic>
#include <exception>
#include <filesystem>
#include <queue>
#include <shared_mutex>
#include <thread>

namespace nate::Modules::Render {
    class Renderer
        : public IRenderer
        , private Jobs::Job {
      public:
        virtual ~Renderer() { PrivShutdown(); }

        void Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc) override;
        bool IsInitialized() const override { return m_RendererInitialized; }
        bool IsRunning() const override { return IsExecuting(); }
        void Stop() override { Jobs::Job::Stop(); }
        void Shutdown() override { PrivShutdown(); }
        bool WaitingForShutdown() const override { return m_WaitingForShutdown; }

        bool                  RenderingFailed() const override { return IsFailed(); }
        const std::exception& GetFailure() const override { return GetCaughtException(); }

        // Must be called during App::UpdateApp or App::Initialize
        void AttachCamera(const Camera3D* pCamera) override { m_pCamera = pCamera; }
        // Must be called during App::UpdateApp or App::Initialize
        void RenderObject(const Object3D* pObject) override { m_Objects.push(pObject); }

        void RenderFrame() override;

      protected:
        void ExecuteJob() final;

      private:
        void                  PrivShutdown();
        GUI::IWindow*         m_pWindow{nullptr};
        std::filesystem::path m_ShaderLoc;

        std::atomic<bool>           m_WindowShouldClose{false};
        std::atomic<bool>           m_RendererInitialized{false};
        std::atomic<bool>           m_ShouldShutdown{false};
        std::atomic<bool>           m_WaitingForShutdown{false};
        std::queue<const Object3D*> m_Objects;
        const Camera3D*             m_pCamera;
    };
} // namespace nate::Modules::Render
