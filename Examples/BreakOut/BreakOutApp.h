#pragma once

#include <3D/Camera.h>
#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <Entity.h>
#include <World.h>

#include <memory>

namespace nate::BreakOut
{
    class BreakOutApp : public Modules::App::App
    {

        std::unique_ptr<Modules::Render::Camera>                                               m_pCamera;
        std::unique_ptr<Modules::Render::Sprite>                                               m_pSprite;
        Modules::Render::ShaderProgram_ptr                                                     m_pShader;
        std::unique_ptr<Modules::ECS::World<Modules::Render::Mesh3D, Modules::Render::Sprite>> m_pWorld;

      public:
        BreakOutApp(
            std::unique_ptr<Modules::ECS::World<Modules::Render::Mesh3D, Modules::Render::Sprite>> pWorld,
            const Modules::GUI::WindowSize&                                                        window_size,
            std::string                                                                            window_name);

      protected:
        void Initialize() override;
        void Shutdown() override;
        void UpdateApp(std::chrono::nanoseconds time) override;
    };
} // namespace nate::BreakOut
