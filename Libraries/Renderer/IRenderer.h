#pragma once

#include "Camera/Camera.h"
#include "IVertexBuffer.h"
#include "IWindow.h"
#include "Objects/Mesh.h"
#include "Shader/IShader.h"
#include "Shader/IShaderProgram.h"
#include "Texture/ITexture.h"
#include "Texture/ImageFile.h"
#include "VertexDataConfig.h"

#include <System.hpp>

#include <memory>
#include <span>

namespace Ignosi::Libraries::Renderer
{
    class IRenderer : public ECS::System<Mesh>
    {
      public:
        IRenderer()          = default;
        virtual ~IRenderer() = default;

        IRenderer(const IRenderer& other)     = delete;
        IRenderer(IRenderer&& other) noexcept = delete;

        IRenderer& operator=(const IRenderer& other)     = delete;
        IRenderer& operator=(IRenderer&& other) noexcept = delete;

        static std::unique_ptr<IRenderer> Create();

        virtual IWindow* AttachedWindow() const                         = 0;
        virtual IWindow* AttachWindow(std::unique_ptr<IWindow> pWindow) = 0;

        virtual void                           AttachedCamera(std::shared_ptr<Camera> camera) = 0;
        virtual const std::shared_ptr<Camera>& AttachedCamera() const                         = 0;

        virtual std::unique_ptr<IVertexBuffer> CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) = 0;
        virtual std::unique_ptr<IVertexBuffer> CreateBuffer(
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces) = 0;

        virtual std::unique_ptr<IShader> CreateShader(
            const std::filesystem::path&              path,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) = 0;
        virtual std::unique_ptr<IShader> CreateShader(
            const std::filesystem::path&              path,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) = 0;

        virtual std::unique_ptr<IShaderProgram> CreateShaderProgram(
            const IShader* pFragmentShader,
            const IShader* pGeometryShader,
            const IShader* pVertexShader) = 0;

        virtual std::unique_ptr<ITexture> CreateTexture(
            const std::string&           textureName,
            const std::filesystem::path& path,
            TextureUnit                  unit)                                                                                                     = 0;
        virtual std::unique_ptr<ITexture> CreateTexture(const std::string& textureName, const ImageFile& image, TextureUnit unit) = 0;

        virtual void ClearDepthBuffer() = 0;
        virtual void ClearColorBuffer() = 0;
        virtual void SwapBuffers()      = 0;
    };
} // namespace Ignosi::Libraries::Renderer
