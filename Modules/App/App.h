#pragma once

#include <IWindow.h>
#include <IWorld.h>

#include <cstddef>
#include <memory>

namespace Ignosi::Modules::App
{
    class App
    {
        GUI::IWindow*                m_pWindow;
        std::unique_ptr<ECS::IWorld> m_pWorld;

      public:
        App(std::unique_ptr<ECS::IWorld> pWorld);
        ~App() = default;

        int  Run();
        void Close();

      protected:
        GUI::IWindow*         GetWindow() const { return m_pWindow; }
        ECS::IWorld*          GetWorld() const { return m_pWorld.get(); }
        virtual void          UpdateApp(double dt) = 0;
        virtual GUI::IWindow* Initialize()         = 0;
        virtual void          Shutdown()           = 0;
    };
} // namespace Ignosi::Modules::App