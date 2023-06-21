#include <IWindow.h>
#include <Renderer/Renderer.h>

#include <memory>

namespace nate::Modules::App
{
    class App
    {
        GUI::IWindow*     m_pWindow;
        Render::Renderer* m_pRenderer;

      public:
        App(std::unique_ptr<Render::Renderer> pRenderer, const GUI::WindowSize& window_size, std::string window_name);
        ~App() = default;

        int  Run();
        void Close();

      protected:
        GUI::IWindow*     GetWindow() const { return m_pWindow; }
        virtual void      UpdateApp(std::chrono::nanoseconds time) = 0;
        virtual void      Initialize()                             = 0;
        virtual void      Shutdown()                               = 0;
    }; // namespace nate::Modules::App
} // namespace nate::Modules::App