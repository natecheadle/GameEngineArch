#pragma once

#include <App.h>
#include <IWindow.h>
#include <Objects/Mesh.h>
#include <World.hpp>

#include <chrono>

namespace Ignosi::Example
{
    using TestWorld  = Libraries::ECS::World<Libraries::Renderer::Mesh>;
    using TestEntity = Libraries::ECS::Entity<Libraries::Renderer::Mesh>;

    class HelloCubeApp : public Libraries::ECS::App
    {
        TestWorld                             m_World;
        Libraries::ECS::ECSObject<TestEntity> m_Entity;

        std::chrono::milliseconds m_UpdatePeriod{17};

        unsigned int                  m_Update = 0;
        Libraries::Renderer::IWindow* m_pWindow;

      public:
        HelloCubeApp()  = default;
        ~HelloCubeApp() = default;

      protected:
        bool KeepUpdating() override;

        void Initialize() override;
        void Update() override;
        void ShutDown() override;
    };
} // namespace Ignosi::Example
