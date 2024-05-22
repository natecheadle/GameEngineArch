#pragma once

#include "IRenderer.h"

#include <chrono>

namespace Ignosi::Libraries::Renderer
{
    class OpenGL_Renderer : public IRenderer
    {
        std::unique_ptr<IWindow> m_pWindow;
        std::shared_ptr<Camera>  m_pCamera;

      public:
        OpenGL_Renderer()           = default;
        ~OpenGL_Renderer() override = default;

        IWindow* AttachedWindow() const override;
        IWindow* AttachWindow(std::unique_ptr<IWindow> pWindow) override;

        void                           AttachedCamera(std::shared_ptr<Camera> camera) override;
        const std::shared_ptr<Camera>& AttachedCamera() const override;

        std::unique_ptr<IVertexBuffer> CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) const override;
        std::unique_ptr<IVertexBuffer> CreateBuffer(
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces) const override;

        std::unique_ptr<IShader> CreateShader(
            const std::filesystem::path&              path,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) const override;
        std::unique_ptr<IShader> CreateShader(
            const std::filesystem::path&              path,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) const override;

        std::unique_ptr<IShaderProgram> CreateShaderProgram(
            const IShader* pFragmentShader,
            const IShader* pGeometryShader,
            const IShader* pVertexShader) const override;

        std::unique_ptr<ITexture> CreateTexture(const std::filesystem::path& path, TextureUnit unit) const override;
        std::unique_ptr<ITexture> CreateTexture(const ImageFile& image, TextureUnit unit) const override;

        void ClearDepthBuffer() const override;
        void ClearColorBuffer() const override;
        void SwapBuffers() const override;

        void Update(std::chrono::milliseconds delta) override;
    };
} // namespace Ignosi::Libraries::Renderer
