#include "3D/Light_Directional.h"
#include "3D/Light_Point.h"
#include "3D/Light_Spotlight.h"
#include "3D/Material.h"
#include "3D/Object3D.h"
#include "IWindow.h"
#include "Renderer/Renderer.h"
#include "Vector3.hpp"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <3D/Fly_Camera3D.h>
#include <App.h>
#include <DebugCast.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <Shader/Shader.h>

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

class TestApp : public App::App
{
    std::mutex                                     m_CallbackMutex;
    GUI::MouseClickedInfo                          m_LastMouseClick;
    GUI::CursorPosition                            m_LastPosition;
    GUI::WindowSize                                m_WindowSize;
    float                                          m_CamYaw{0.0};
    float                                          m_CamPitch{0.0};
    std::vector<std::unique_ptr<Render::Object3D>> m_Cubes;
    Render::Light_Directional                      m_DirLight;
    Render::Light_Spotlight                        m_SpotLight;
    Render::Light_Point                            m_PointLight;
    Render::Material                               m_CubeMaterial;
    std::unique_ptr<Render::Fly_Camera3D>          m_pCamera;

  public:
    TestApp(std::unique_ptr<Render::Renderer> pRenderer, const GUI::WindowSize& window_size, std::string window_name)
        : App(std::move(pRenderer), window_size, std::move(window_name))
    {

        GetWindow()->SubscribeToMessage(
            this,
            GUI::WindowMessages::MouseClicked,
            [this](const GUI::WindowMessage* pMessage) {
                std::unique_lock<std::mutex> lock(m_CallbackMutex);

                m_LastMouseClick = *(DebugCast<const GUI::MouseClicked*>(pMessage)->GetData());
            });
        GetWindow()->SubscribeToMessage(
            this,
            GUI::WindowMessages::WindowResized,
            [this](const GUI::WindowMessage* pMessage) {
                std::unique_lock<std::mutex> lock(m_CallbackMutex);

                m_WindowSize = *(DebugCast<const GUI::WindowResized*>(pMessage)->GetData());
            });
    }

  protected:
    void Initialize() override
    {
        std::filesystem::path shader_dir(std::string(SHADER_DIR));
        shader_dir /= "Shaders";

        auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
        auto fragment_shader_path = shader_dir / "fragment_shader.frag";
        auto light_source_path    = shader_dir / "light_source.frag";
        auto cont_spec_path       = shader_dir / "container2_specular.png";
        auto cont_path            = shader_dir / "container2.png";

        auto pContTex     = GetRenderer()->CreateTexture(cont_path, nate::Modules::Render::TextureUnit::Texture0);
        auto pContSpecTex = GetRenderer()->CreateTexture(cont_spec_path, nate::Modules::Render::TextureUnit::Texture1);

        auto pVertexShader   = GetRenderer()->CreateShader(vertex_shader_path);
        auto pFragmentShader = GetRenderer()->CreateShader(fragment_shader_path);
        auto pProgram        = GetRenderer()->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

        const size_t numOfCubes{10};
        m_Cubes.resize(numOfCubes);

        m_Cubes[0] = Render::Object3D::CreateCube(GetRenderer());
        m_Cubes[0]->Shader(std::move(pProgram));
        m_Cubes[0]->Textures({std::move(pContTex), std::move(pContSpecTex)});

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
            m_Cubes[i] = std::make_unique<Render::Object3D>(*m_Cubes[0]);
            m_Cubes[i]->Origin(cubePositions[i]);
        }

        m_pCamera = std::make_unique<Render::Fly_Camera3D>(GetWindow());

        m_DirLight.Direction = {0.0f, 0.0f, -1.0f};
        m_DirLight.Ambient   = {0.2f, 0.2f, 0.2f};
        m_DirLight.Diffuse   = {0.5f, 0.5f, 0.5f};
        m_DirLight.Specular  = {1.0f, 1.0f, 1.0f};

        m_SpotLight.Position              = m_pCamera->CameraPosition();
        m_SpotLight.Direction             = m_pCamera->CameraDirection();
        m_SpotLight.Ambient               = {0.0f, 0.0f, 0.0f};
        m_SpotLight.Diffuse               = {1.0f, 1.0f, 1.0f};
        m_SpotLight.Specular              = {1.0f, 1.0f, 1.0f};
        m_SpotLight.Attenuation.Constant  = 1.0f;
        m_SpotLight.Attenuation.Linear    = 0.09f;
        m_SpotLight.Attenuation.Quadratic = 0.32f;
        m_SpotLight.Cutoff                = 12.5f;
        m_SpotLight.OuterCutoff           = 15.0f;

        m_PointLight.Position              = {0.7f, 0.2f, 2.0f};
        m_PointLight.Ambient               = {0.05f, 0.05f, 0.05f};
        m_PointLight.Diffuse               = {0.8f, 0.8f, 0.8f};
        m_PointLight.Specular              = {1.0f, 1.0f, 1.0f};
        m_PointLight.Attenuation.Constant  = 1.0f;
        m_PointLight.Attenuation.Linear    = 0.09f;
        m_PointLight.Attenuation.Quadratic = 0.32f;

        m_CubeMaterial.Shininess = 64.0f;

        auto* pRenderer = GetRenderer();
        for (auto& cube : m_Cubes)
        {
            pRenderer->SetShaderVar(cube->Shader().get(), "material.diffuse", 0);
            pRenderer->SetShaderVar(cube->Shader().get(), "material.specular", 1);
            pRenderer->SetShaderVar(cube->Shader().get(), "material.shininess", m_CubeMaterial.Shininess);

            pRenderer->SetShaderVar(cube->Shader().get(), "dirLight.ambient", m_DirLight.Ambient.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "dirLight.diffuse", m_DirLight.Diffuse.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "dirLight.specular", m_DirLight.Specular.Data());

            pRenderer->SetShaderVar(cube->Shader().get(), "pointLight.ambient", m_PointLight.Ambient.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "pointLight.diffuse", m_PointLight.Diffuse.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "pointLight.specular", m_PointLight.Specular.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "pointLight.constant", m_PointLight.Attenuation.Constant);
            pRenderer->SetShaderVar(cube->Shader().get(), "pointLight.diffuse", m_PointLight.Attenuation.Linear);
            pRenderer->SetShaderVar(cube->Shader().get(), "pointLight.quadratic", m_PointLight.Attenuation.Quadratic);

            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.ambient", m_SpotLight.Ambient.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.diffuse", m_SpotLight.Diffuse.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.specular", m_SpotLight.Specular.Data());
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.constant", m_SpotLight.Attenuation.Constant);
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.diffuse", m_SpotLight.Attenuation.Linear);
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.quadratic", m_SpotLight.Attenuation.Quadratic);
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.cutOff", cos(m_SpotLight.Cutoff));
            pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.outerCutOff", cos(m_SpotLight.OuterCutoff));
        }
    }

    void Shutdown() override
    {
        m_pCamera.reset();
        m_Cubes.clear();
    }

    void UpdateApp(std::chrono::nanoseconds time) override
    {
        static std::chrono::nanoseconds totalTime;
        totalTime += time;
        // TODO this should be handled automatically
        m_pCamera->Update(time);
        auto* pRenderer = GetRenderer();
        for (auto& cube : m_Cubes)
        {
            cube->RotX(M_PI / 500.0);
        }

        auto renderUpdate = [&]() -> void {
            for (const auto& cube : m_Cubes)
            {
                pRenderer->SetShaderVar(cube->Shader().get(), "model", cube->ModelMatrix());
                pRenderer->SetShaderVar(cube->Shader().get(), "view", m_pCamera->View());
                pRenderer->SetShaderVar(cube->Shader().get(), "norm_mat", cube->NormalMatrix());
                pRenderer->SetShaderVar(cube->Shader().get(), "viewPos", m_pCamera->CameraPosition());
                pRenderer->SetShaderVar(cube->Shader().get(), "projection", m_pCamera->Projection());

                pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.position", m_pCamera->CameraPosition());
                pRenderer->SetShaderVar(cube->Shader().get(), "spotLight.direction", -1 * m_pCamera->CameraDirection());

                pRenderer->Draw(cube.get());
            }
        };

        pRenderer->ExecuteFunction(renderUpdate);
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
