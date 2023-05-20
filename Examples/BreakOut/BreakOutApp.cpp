#include "BreakOutApp.h"

#include <Renderer/Renderer_OpenGL.h>
#include <Units/Degree.hpp>

using namespace nate::Modules;
using namespace std::chrono_literals;

namespace nate::BreakOut
{
    BreakOutApp::BreakOutApp(
        std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite>> pWorld,
        const GUI::WindowSize&                                      window_size,
        std::string                                                 window_name)
        : App(pWorld->CreateSystem<Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>(),
              window_size,
              std::move(window_name))
        , m_pWorld(std::move(pWorld))
    {
    }

    void BreakOutApp::Initialize()
    {
        std::filesystem::path shader_dir(std::string_view(SHADER_DIR));
        std::filesystem::path shader_inc_dir = std::filesystem::path(std::string_view(SHADER_DIR));
        shader_dir /= "Shaders";
        shader_inc_dir /= "../../Modules/Render/Shader/GLSL/";
        assert(std::filesystem::is_directory(shader_dir));
        assert(std::filesystem::is_directory(shader_inc_dir));

        auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
        auto fragment_shader_path = shader_dir / "fragment_shader.frag";
        auto face_path            = shader_dir / "awesomeface.png";

        auto cubeMaterial = std::make_unique<Render::Material>();

        cubeMaterial->Diffuse =
            GetRenderer()->CreateTexture({face_path, false}, nate::Modules::Render::TextureUnit::Texture0);

        auto pVertexShader   = GetRenderer()->CreateShader(vertex_shader_path, {shader_inc_dir});
        auto pFragmentShader = GetRenderer()->CreateShader(fragment_shader_path, {shader_inc_dir});
        m_pShader            = GetRenderer()->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

        auto* pRenderer = GetRenderer();
        m_pCamera       = std::make_unique<Render::Camera>(GetWindow());
        m_pCamera->Near(-1.0);
        m_pCamera->Far(1.0);

        m_pSprite = std::make_unique<Render::Sprite>(pRenderer);
        m_pSprite->AttachedMaterial(std::move(cubeMaterial));

        auto renderUpdate = [&]() -> void {
            m_pShader->Use();
            m_pShader->SetShaderVar("projection", m_pCamera->ViewOrthographic());
            m_pShader->SetShaderVar("material", *m_pSprite->AttachedMaterial());
        };

        GetRenderer()->ExecuteFunction(renderUpdate);
    }

    void BreakOutApp::Shutdown()
    {
        m_pCamera.reset();
        m_pSprite.reset();
        m_pShader.reset();
    }

    void BreakOutApp::UpdateApp(std::chrono::nanoseconds time)
    {
        // TODO this should be handled automatically
        m_pCamera->Update(time);

        m_pSprite->Origin({200.0, 200.0});
        m_pSprite->Size({300.0f, 400.0f});
        m_pSprite->Rotation(Degree<float>(45.0));
        m_pSprite->Color({0.0f, 1.0f, 0.0f});

        auto renderUpdate = [&]() -> void {
            m_pShader->Use();
            m_pSprite->Draw(m_pShader.get());
        };

        GetRenderer()->ExecuteFunction(renderUpdate);
    }
} // namespace nate::BreakOut
