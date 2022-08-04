#include <IRenderer.h>
#include <IWindow.h>

#include <memory>

namespace nate::Modules::App {
    class App {
        std::unique_ptr<GUI::IWindow>      m_pWindow;
        std::unique_ptr<Render::IRenderer> m_pRenderer;

      public:
        App(std::unique_ptr<GUI::IWindow> pWindow, std::unique_ptr<Render::IRenderer> pRenderer);
        ~App() { Shutdown(); }

        int Run();

        void Shutdown();

      protected:
        GUI::IWindow*      GetWindow() const { return m_pWindow.get(); }
        Render::IRenderer* GetRenderer() const { return m_pRenderer.get(); }
        virtual void       UpdateApp() = 0;
    }; // namespace nate::Modules::App
} // namespace nate::Modules::App