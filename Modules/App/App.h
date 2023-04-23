#include <IWindow.h>
#include <Renderer.h>

#include <memory>

namespace nate::Modules::App
{
    class App
    {
        std::unique_ptr<GUI::IWindow>     m_pWindow;
        std::unique_ptr<Render::Renderer> m_pRenderer;

      public:
        App(std::unique_ptr<GUI::IWindow> pWindow, std::unique_ptr<Render::Renderer> pRenderer);
        ~App() = default;

        int  Run();
        void Close();

      protected:
        GUI::IWindow*     GetWindow() const { return m_pWindow.get(); }
        Render::Renderer* GetRenderer() const { return m_pRenderer.get(); }
        virtual void      UpdateApp(std::chrono::nanoseconds time) = 0;
        virtual void      Initialize()                             = 0;
        virtual void      Shutdown()                               = 0;
    }; // namespace nate::Modules::App
} // namespace nate::Modules::App