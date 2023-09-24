#pragma once

#include "../3D/Mesh3D.h"
#include "Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Tag.h"
#include "Texture/Texture.h"

#include <Window_GLFW.h>

#include <memory>
#include <string_view>

namespace Ignosi::Modules::Render
{

    class Renderer_OpenGL : public Renderer
    {
        std::unique_ptr<GUI::Window_GLFW> m_pWin;
        static constexpr std::string_view NAME = "Renderer_OpenGL";

      public:
        Renderer_OpenGL(ECS::ComponentPool<Mesh3D>* pMeshPool, ECS::ComponentPool<Sprite>* pSpritePool);

        ~Renderer_OpenGL() override;

        GUI::IWindow* Window() const override { return m_pWin.get(); }

        GUI::IWindow* InitializeWindow(const GUI::WindowSize& size, std::string name) override;

        std::unique_ptr<VertexBuffer> CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) override;
        std::unique_ptr<VertexBuffer> CreateBuffer(
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces) override;

        Shader* CreateShader(
            const std::filesystem::path&              path,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) final;
        Shader* CreateShader(
            const std::filesystem::path&              path,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) final;

        std::unique_ptr<ShaderProgram> CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) final;

        std::unique_ptr<Texture> CreateTexture(const std::string& textureName, const std::filesystem::path& path, TextureUnit unit) final;
        std::unique_ptr<Texture> CreateTexture(const std::string& textureName, const ImageFile& image, TextureUnit unit) final;

        void ClearDepthBuffer() override;
        void ClearColorBuffer() override;
        void SwapBuffers() override;

        std::string_view Name() const override { return NAME; }
        std::uint32_t    Priority() const override { return 32; }

      private:
        static bool Validate(void* pVoid);

        void OnWindowResized(const GUI::WindowMessage* pMessage);

        template <class T>
        void SetShaderVar_T(ShaderProgram* pShader, const std::string& name, const T& value)
        {
            assert(pShader);
            pShader->SetShaderVar(name, value);
        }
    };
} // namespace Ignosi::Modules::Render
