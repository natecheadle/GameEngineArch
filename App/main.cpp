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
#include <ComponentPointer.h>
#include <CustomEntity.h>
#include <DebugCast.hpp>
#include <Entity.h>
#include <IWindow.h>
#include <IWorld.h>
#include <KinematicData.h>
#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector3.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <PhysicsSystem.h>
#include <Renderer/Renderer_OpenGL.h>
#include <RigidBody2D.h>
#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>
#include <Units/AngularAcceleration.hpp>
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
#include <numbers>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

using namespace Ignosi::Modules;
using namespace std::chrono_literals;

class TestAppEntity : public ECS::CustomEntity<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>
{
    using BASE = ECS::CustomEntity<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>;

  public:
    TestAppEntity() = default;
    TestAppEntity(ECS::EntityPointer<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>&& val)
        : BASE(std::move(val))
    {
    }
    TestAppEntity(const TestAppEntity& other)     = delete;
    TestAppEntity(TestAppEntity&& other) noexcept = default;

    TestAppEntity& operator=(const TestAppEntity& other)     = delete;
    TestAppEntity& operator=(TestAppEntity&& other) noexcept = default;

    ECS::WeakComponentPointer<Render::Mesh3D>         Mesh() { return BASE::GetComponent<Render::Mesh3D>(); }
    ECS::WeakComponentPointer<Physics::KinematicData> KinematicData() { return BASE::GetComponent<Physics::KinematicData>(); }

  protected:
    void OnUpdate(double dt) override {}
};

class TestApp : public App::App
{
    std::vector<TestAppEntity>                                                                m_Cubes;
    Render::Light_Directional                                                                 m_DirLight;
    Render::Light_Spotlight                                                                   m_SpotLight;
    Render::Light_Point                                                                       m_PointLight;
    std::shared_ptr<Render::Fly_Camera>                                                       m_pCamera;
    std::shared_ptr<Render::ShaderProgram>                                                    m_pShader;
    ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>* m_pWorld;
    Render::Renderer*                                                                         m_pRenderer;
    Physics::PhysicsSystem*                                                                   m_pPhysicsSystem;
    GUI::IWindow*                                                                             m_pWindow;

  public:
    TestApp(std::unique_ptr<ECS::IWorld> pWorld, const GUI::WindowSize& window_size, std::string window_name)
        : App(std::move(pWorld))
    {
        m_pWorld = dynamic_cast<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>*>(App::GetWorld());
        m_pRenderer      = m_pWorld->CreateSystem<Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>();
        m_pPhysicsSystem = m_pWorld->CreateSystem<Physics::PhysicsSystem, Physics::RigidBody2D, Physics::KinematicData>();
        m_pWindow        = m_pRenderer->InitializeWindow(window_size, std::move(window_name));
    }

  protected:
    GUI::IWindow* Initialize() override
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

        auto cubeMaterial = std::make_shared<Render::Material>();

        cubeMaterial->Diffuse = m_pRenderer->CreateTexture("cube_material_1", cont_path, Ignosi::Modules::Render::TextureUnit::Texture0);
        cubeMaterial->Specular =
            m_pRenderer->CreateTexture("cube_material_2", cont_spec_path, Ignosi::Modules::Render::TextureUnit::Texture1);
        cubeMaterial->Shininess = 64.0;

        auto* pVertexShader   = m_pRenderer->CreateShader(vertex_shader_path, {shader_inc_dir});
        auto* pFragmentShader = m_pRenderer->CreateShader(fragment_shader_path, {shader_inc_dir});
        m_pShader             = m_pRenderer->CreateShaderProgram(pFragmentShader, nullptr, pVertexShader);

        const size_t numOfCubes{10};
        m_Cubes.reserve(numOfCubes);

        const Vector3<float> cubePositions[] = {
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
        const RadianPerSecond<float> rotSpeed(static_cast<float>(M_PI / 10.0));
        for (size_t i = 0; i < numOfCubes; ++i)
        {
            m_Cubes.push_back(TestAppEntity(m_pWorld->CreateEntity()));
            m_pWorld->AddComponent<Physics::KinematicData>(m_Cubes[i].Entity());
            m_pWorld->AddComponent<Render::Mesh3D>(
                m_Cubes[i].Entity(),
                Render::Mesh3D::CreateCube(m_pRenderer, m_Cubes[i].KinematicData()));
            m_Cubes[i].KinematicData()->Position(cubePositions[i]);
            m_Cubes[i].KinematicData()->AngularVelocity({rotSpeed, 0.0, 0.0});
            m_Cubes[i].Mesh()->Shader(m_pShader);
            m_Cubes[i].Mesh()->AttachedMaterial(cubeMaterial);
        }

        m_pCamera = std::make_shared<Render::Fly_Camera>(m_pWindow);
        m_pRenderer->AttachedCamera(m_pCamera);

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

        // m_pBackpackModel = std::make_unique<Render::Model3D>(m_pRenderer, backpack_path);

        return m_pWindow;
    }

    void Shutdown() override
    {
        m_pCamera.reset();
        m_Cubes.clear();
    }

    void UpdateApp(double dt) override
    {
        m_pShader->Use();
        m_SpotLight.Position  = m_pCamera->CameraPosition();
        m_SpotLight.Direction = -1.0 * m_pCamera->CameraDirection();

        m_pShader->SetShaderVar("dirLight", m_DirLight);
        m_pShader->SetShaderVar("pointLight", m_PointLight);
        m_pShader->SetShaderVar("spotLight", m_SpotLight);

        m_pWorld->Update(dt);
    }
};

int main()
{
    try
    {
        TestApp app(
            std::make_unique<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>>(),
            {800, 600},
            "Test Window");
        int code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
