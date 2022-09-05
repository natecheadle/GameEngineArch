#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"
#include "CursorPosition.hpp"
#include "IRenderer.h"
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
#include <shared_mutex>
#include <thread>

namespace nate::Modules::Render
{
    class RendererSM;
    class Renderer
        : public IRenderer
        , private Jobs::Job
    {
      public:
        virtual ~Renderer();

        void Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc) override;
        bool IsRunning() const override { return IsExecuting(); }
        void StartRendering() override;

        void Shutdown() override;

        bool                  RenderingFailed() const override { return IsFailed(); }
        const std::exception& GetFailure() const override { return GetCaughtException(); }

        // Must be called during App::UpdateApp
        void AttachCamera(std::shared_ptr<const Camera3D> pCamera) override;
        // Must be called during App::UpdateApp
        void RenderObject(std::shared_ptr<const Object3D> pObject) override;

        void RenderFrame() override;

      protected:
        void ExecuteJob() final;

      private:
        void PrivShutdown();

        std::unique_ptr<RendererSM> m_pRendererSM;
    };
} // namespace nate::Modules::Render
