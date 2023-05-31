#include <3D/Camera2D.h>
#include <3D/Fly_Camera.h>
#include <3D/Light_Directional.h>
#include <3D/Light_Point.h>
#include <3D/Light_Spotlight.h>
#include <3D/Material.h>
#include <3D/Mesh3D.h>
#include <3D/Model3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <DebugCast.hpp>
#include <Entity.h>
#include <IWindow.h>
#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector3.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <Renderer/Renderer.h>
#include <Renderer/Renderer_OpenGL.h>
#include <Shader/Shader.h>
#include <WindowMessages.hpp>
#include <WindowSize.hpp>
#include <World.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

using namespace nate::Modules;
using namespace std::chrono_literals;

class TestAppEntity : ECS::Entity<Render::Mesh3D>
{
  public:
    TestAppEntity(Memory::pool_pointer<Render::Mesh3D>&& val)
        : ECS::Entity<Render::Mesh3D>(std::move(val))
    {
    }

    Render::Mesh3D&       Mesh() { return ECS::Entity<Render::Mesh3D>::Get<Render::Mesh3D>(); }
    const Render::Mesh3D& Mesh() const { return ECS::Entity<Render::Mesh3D>::Get<Render::Mesh3D>(); }
};

class TestApp : public App::App
{
    std::vector<TestAppEntity>                                  m_Cubes;
    Render::Light_Directional                                   m_DirLight;
    Render::Light_Spotlight                                     m_SpotLight;
    Render::Light_Point                                         m_PointLight;
    std::unique_ptr<Render::Fly_Camera>                         m_pCamera;
    Render::ShaderProgram_ptr                                   m_pShader;
    std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite>> m_pWorld;

  public:
    TestApp(
        std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite>> pWorld,
        const GUI::WindowSize&                                      window_size,
        std::string                                                 window_name)
        : App(pWorld->CreateSystem<Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>(),
              window_size,
              std::move(window_name))
        , m_pWorld(std::move(pWorld))
    {
    }

  protected:
    void Initialize() override
    {
        std::filesystem::path shader_dir(std::string_view(SHADER_DIR));
        std::filesystem::path shader_inc_dir = std::filesystem::path(std::string_view(SHADER_DIR));
        shader_dir /= "Shaders";
        shader_inc_dir /= "../Modules/Render/Shader/GLSL/";
        assert(std::filesystem::is_directory(shader_dir));
        assert(std::filesystem::is_directory(shader_inc_dir));

        auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
        auto fragment_shader_path = shader_dir / "fragment_shader.frag";
        auto light_source_path    = shader_dir / "light_source.frag";
        auto cont_spec_path       = shader_dir / "container2_specular.png";
        auto cont_path            = shader_dir / "container2.png";
        auto backpack_path        = shader_dir / "backpack/backpack.obj";

        auto cubeMaterial = std::make_unique<Render::Material>();

        cubeMaterial->Diffuse = GetRenderer()->CreateTexture(cont_path, nate::Modules::Render::TextureUnit::Texture0);
        cubeMaterial->Specular =
            GetRenderer()->CreateTexture(cont_spec_path, nate::Modules::Render::TextureUnit::Texture1);
        cubeMaterial->Shininess = 64.0;

        auto pVertexShader   = GetRenderer()->CreateShader(vertex_shader_path, {shader_inc_dir});
        auto pFragmentShader = GetRenderer()->CreateShader(fragment_shader_path, {shader_inc_dir});
        m_pShader            = GetRenderer()->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

        const size_t numOfCubes{10};
        m_Cubes.reserve(numOfCubes);
        m_Cubes.push_back(m_pWorld->CreateEntity<TestAppEntity>(Render::Mesh3D::CreateCube(GetRenderer())));
        m_Cubes[0].Mesh().AttachedMaterial(std::move(cubeMaterial));

        Vector3<float> cubePositions[] = {
            Vector3<float>(0.0f, 0.0f, 0.0f),
            Vector3<float>(2.0f, 5.0f, -15.0f),
            Vector3<float>(-1.5f, -2.2f, -2.5f),
            Vector3<float>(-3.8f, -2.0f, -12.3f),
            Vector3<float>(2.4f, -0.4f, -3.5f),
            Vector3<float>(-1.7f, 3.0f, -7.5f),
            Vector3<float>(1.3f, -2.0f, -2.5f),
            Vector3<float>(1.5f, 2.0f, -2.5f),
            Vector3<float>(1.5f, 0.2f, -1.5f),
            Vector3<float>(-1.3f, 1.0f, -1.5f)};

        static_assert(sizeof(cubePositions) / sizeof(Vector3<float>) == numOfCubes, "Incorrect number of cubes");

        for (size_t i = 1; i < numOfCubes; ++i)
        {
            m_Cubes.push_back(m_pWorld->CreateEntity<TestAppEntity>(Render::Mesh3D(m_Cubes[0].Mesh())));
            m_Cubes[i].Mesh().Origin(cubePositions[i]);
        }

        m_pCamera = std::make_unique<Render::Fly_Camera>(GetWindow());

        m_DirLight.Direction      = {0.0f, 0.0f, -1.0f};
        m_DirLight.Light.Ambient  = {0.2f, 0.2f, 0.2f};
        m_DirLight.Light.Diffuse  = {0.5f, 0.5f, 0.5f};
        m_DirLight.Light.Specular = {1.0f, 1.0f, 1.0f};

        m_SpotLight.Position              = m_pCamera->CameraPosition();
        m_SpotLight.Direction             = m_pCamera->CameraDirection();
        m_SpotLight.Light.Ambient         = {0.0f, 0.0f, 0.0f};
        m_SpotLight.Light.Diffuse         = {1.0f, 1.0f, 1.0f};
        m_SpotLight.Light.Specular        = {1.0f, 1.0f, 1.0f};
        m_SpotLight.Attenuation.Constant  = 1.0f;
        m_SpotLight.Attenuation.Linear    = 0.09f;
        m_SpotLight.Attenuation.Quadratic = 0.32f;
        m_SpotLight.Cutoff                = 12.5f;
        m_SpotLight.OuterCutoff           = 15.0f;

        m_PointLight.Position              = {0.7f, 0.2f, 2.0f};
        m_PointLight.Light.Ambient         = {0.05f, 0.05f, 0.05f};
        m_PointLight.Light.Diffuse         = {0.8f, 0.8f, 0.8f};
        m_PointLight.Light.Specular        = {1.0f, 1.0f, 1.0f};
        m_PointLight.Attenuation.Constant  = 1.0f;
        m_PointLight.Attenuation.Linear    = 0.09f;
        m_PointLight.Attenuation.Quadratic = 0.32f;

        auto* pRenderer      = GetRenderer();
        auto  initShaderVars = [&]() -> void {
            m_pShader->Use();
            m_pShader->SetShaderVar("dirLight", m_DirLight);
            m_pShader->SetShaderVar("pointLight", m_PointLight);
            m_pShader->SetShaderVar("spotLight", m_SpotLight);

            for (auto& cube : m_Cubes)
            {
                m_pShader->SetShaderVar("material", *(cube.Mesh().AttachedMaterial()));
            }
        };
        pRenderer->ExecuteFunction(initShaderVars);

        // m_pBackpackModel = std::make_unique<Render::Model3D>(pRenderer, backpack_path);
    }

    void Shutdown() override
    {
        m_pCamera.reset();
        m_Cubes.clear();
    }

    void UpdateApp(std::chrono::nanoseconds time) override
    {
        // TODO this should be handled automatically
        m_pCamera->Update(time);
        auto* pRenderer = GetRenderer();
        for (auto& cube : m_Cubes)
        {
            cube.Mesh().RotX(M_PI / 500.0);
        }
        // m_pBackpackModel->RotY(M_PI / 500.0);

        m_SpotLight.Position  = m_pCamera->CameraPosition();
        m_SpotLight.Direction = -1.0 * m_pCamera->CameraDirection();

        auto renderUpdate = [&]() -> void {
            m_pShader->Use();

            m_pShader->SetShaderVar("view", m_pCamera->ViewPerspective());
            m_pShader->SetShaderVar("viewPos", m_pCamera->CameraPosition());
            m_pShader->SetShaderVar("projection", m_pCamera->Projection());

            m_pShader->SetShaderVar("spotLight", m_SpotLight);

            pRenderer->DrawAllMesh(m_pShader.get());
        };

        pRenderer->ExecuteFunction(renderUpdate);
    }
};

int main()
{
    try
    {
        TestApp app(std::make_unique<ECS::World<Render::Mesh3D, Render::Sprite>>(), {800, 600}, "Test Window");
        int     code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
