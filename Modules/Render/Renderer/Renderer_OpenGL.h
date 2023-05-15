#pragma once

#include "../3D/Object3D.h"
#include "Renderer.h"
#include "Renderer/VertexBuffer.h"

#include <Window_GLFW.h>

#include <memory>

namespace nate::Modules::Render
{

    class Renderer_OpenGL : public Renderer
    {
        std::unique_ptr<GUI::Window_GLFW> m_pWin;

      public:
        Renderer_OpenGL() = default;
        ~Renderer_OpenGL() override;

        GUI::IWindow* Window() const override { return m_pWin.get(); }

        GUI::IWindow* Initialize(const GUI::WindowSize& size, std::string name) override;

        VertexBuffer_ptr CreateBuffer(const VertexDataConfig& config, std::span<float> vertexes) override;
        VertexBuffer_ptr CreateBuffer(
            const VertexDataConfig&  config,
            std::span<float>         vertexes,
            std::span<std::uint32_t> indeces) override;

        Shader_ptr CreateShader(const std::filesystem::path& path) final;
        Shader_ptr CreateShader(const std::filesystem::path& path, ShaderType type) final;

        ShaderProgram_ptr CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) final;

        Texture_ptr CreateTexture(const std::filesystem::path& path, TextureUnit unit) final;
        Texture_ptr CreateTexture(const ImageFile& image, TextureUnit unit) final;

        void Draw(Object3D* pObj) final;

        void SetShaderVar(ShaderProgram* pShader, const std::string& name, bool value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, int value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, float value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const SquareMatrix<4, float>& value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<3, float>& value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<4, float>& value) final;

        void ClearDepthBuffer() override;
        void ClearColorBuffer() override;
        void SwapBuffers() override;

      private:
        static bool Validate(void* pVoid);

        void Destroy(VertexBuffer* pObj);
        void Destroy(Shader* pShader);
        void Destroy(Texture* pTex);
        void Destroy(ShaderProgram* pProgram);
    };
} // namespace nate::Modules::Render