#include "Units/Radian.hpp"

#include <3D/Camera.h>
#include <3D/Sprite.h>
#include <App.h>
#include <Renderer/Renderer.h>
#include <SquareMatrix.hpp>
#include <SquareMatrix4x4.hpp>
#include <Units/Degree.hpp>

#include <memory>

using namespace nate::Modules;
using namespace std::chrono_literals;

class TestApp : public App::App
{

    std::unique_ptr<Render::Camera> m_pCamera;
    std::unique_ptr<Render::Sprite> m_pSprite;
    Render::ShaderProgram_ptr       m_pShader;

  public:
    TestApp(std::unique_ptr<Render::Renderer> pRenderer, const GUI::WindowSize& window_size, std::string window_name)
        : App(std::move(pRenderer), window_size, std::move(window_name))
    {
    }

  protected:
    void Initialize() override
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

    void Shutdown() override
    {
        m_pCamera.reset();
        m_pSprite.reset();
        m_pShader.reset();
    }

    void UpdateApp(std::chrono::nanoseconds time) override
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
};

int main()
{
    try
    {
        std::unique_ptr<Render::Renderer> pRenderer = Render::Renderer::Create();
        TestApp                           app(std::move(pRenderer), {800, 600}, "Test Window");
        int                               code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
