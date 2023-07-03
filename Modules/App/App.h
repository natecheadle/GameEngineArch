#include <IWindow.h>
#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>

#include <cstddef>
#include <memory>

namespace Ignosi::Modules::App
{
    class App
    {
        GUI::IWindow*                           m_pWindow;
        Render::Renderer*                       m_pRenderer;
        std::unique_ptr<Physics::PhysicsSystem> m_pPhysics;

      public:
        App(std::unique_ptr<Render::Renderer>       pRenderer,
            const GUI::WindowSize&                  window_size,
            std::string                             window_name,
            std::unique_ptr<Physics::PhysicsSystem> pPhysics = nullptr);
        ~App() = default;

        int  Run();
        void Close();

      protected:
        GUI::IWindow* GetWindow() const { return m_pWindow; }
        virtual void  UpdateApp(double dt) = 0;
        virtual void  Initialize()         = 0;
        virtual void  Shutdown()           = 0;
    };
} // namespace Ignosi::Modules::App