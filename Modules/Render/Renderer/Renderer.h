#pragma once

#include "3D/Camera.h"
#include "3D/Light_Directional.h"
#include "3D/Light_Point.h"
#include "3D/Light_Spotlight.h"
#include "3D/Material.h"
#include "3D/Mesh3D.h"
#include "3D/Model3D.h"
#include "3D/Sprite.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"
#include "VertexBuffer.h"

#include <IWindow.h>
#include <System.h>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <queue>
#include <span>

namespace Ignosi::Modules::Render
{
    class Renderer : public ECS::System<Mesh3D, Sprite>

    {
        std::thread::id                                                  m_RenderThreadID;
        std::mutex                                                       m_QueueMutex;
        std::condition_variable                                          m_QueueCondition;
        std::queue<std::pair<std::promise<void>, std::function<void()>>> m_CommandQueue;
        std::shared_ptr<Camera>                                          m_pCamera;

      protected:
        Renderer(ECS::ComponentPool<Mesh3D>* pMeshPool, ECS::ComponentPool<Sprite>* pSpritePool);

      public:
        virtual ~Renderer();

        virtual GUI::IWindow* Window() const                                        = 0;
        virtual GUI::IWindow* InitializeWindow(const GUI::WindowSize&, std::string) = 0;

        void                           AttachedCamera(std::shared_ptr<Camera> camera) { m_pCamera = std::move(camera); }
        const std::shared_ptr<Camera>& AttachedCamera() const { return m_pCamera; }

        virtual std::unique_ptr<VertexBuffer> CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) = 0;
        virtual std::unique_ptr<VertexBuffer> CreateBuffer(
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces) = 0;

        virtual std::unique_ptr<Shader> CreateShader(
            const std::filesystem::path&              path,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) = 0;
        virtual std::unique_ptr<Shader> CreateShader(
            const std::filesystem::path&              path,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) = 0;

        virtual std::unique_ptr<ShaderProgram> CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) = 0;

        virtual std::unique_ptr<Texture> CreateTexture(const std::filesystem::path& path, TextureUnit unit) = 0;
        virtual std::unique_ptr<Texture> CreateTexture(const ImageFile& image, TextureUnit unit)            = 0;

        virtual void ClearDepthBuffer() = 0;
        virtual void ClearColorBuffer() = 0;
        virtual void SwapBuffers()      = 0;

        void Update(double dt) override;
    };
} // namespace Ignosi::Modules::Render
