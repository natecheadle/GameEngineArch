#include "IRenderer.h"

#include <IWindow.h>
#include <Job.h>

#include <atomic>
#include <thread>

namespace nate::Modules::Render {
    class Renderer
        : public IRenderer
        , private Jobs::Job {
      public:
        virtual ~Renderer() { PrivShutdown(); }

        void Initialize(GUI::IWindow* pWindow) override;
        bool IsRunning() const override { return IsExecuting(); }
        void Shutdown() override { PrivShutdown(); }

        static void RenderFrame();

      protected:
        void ExecuteJob() final;

      private:
        void              PrivShutdown();
        GUI::IWindow*     m_pWindow;
        std::atomic<bool> m_WindowShouldClose;
    };
} // namespace nate::Modules::Render
