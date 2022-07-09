#include "IRenderer.h"

#include <IWindow.h>
#include <Job.h>

#include <thread>

namespace nate::Modules::Render {
    class Renderer
        : public IRenderer
        , private Jobs::Job {
      public:
        virtual ~Renderer() { Stop(); }

        void Initialize(GUI::IWindow* pWindow) override;
        bool IsRunning() const override { return IsExecuting(); }
        void Shutdown() override { Stop(); }

        static void RenderFrame();

      protected:
        void ExecuteJob() final;

      private:
        GUI::IWindow* m_pWindow;
    };
} // namespace nate::Modules::Render
