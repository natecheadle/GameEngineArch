#pragma once

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
#include <Job.h>
#include <Singleton.hpp>
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
    using VertexBuffer_ptr  = std::unique_ptr<VertexBuffer, std::function<void(VertexBuffer*)>>;
    using Shader_ptr        = std::unique_ptr<Shader, std::function<void(Shader*)>>;
    using Texture_ptr       = std::unique_ptr<Texture, std::function<void(Texture*)>>;
    using ShaderProgram_ptr = std::unique_ptr<ShaderProgram, std::function<void(ShaderProgram*)>>;

    class Renderer
        : public ECS::System<Mesh3D, Sprite>
        , protected Jobs::Job
        , public Singleton<Renderer>
    {
        std::thread::id                                                  m_RenderThreadID;
        std::mutex                                                       m_QueueMutex;
        std::condition_variable                                          m_QueueCondition;
        std::queue<std::pair<std::promise<void>, std::function<void()>>> m_CommandQueue;

      protected:
        Renderer(ECS::ComponentPool<Mesh3D>* pMeshPool, ECS::ComponentPool<Sprite>* pSpritePool);

      public:
        virtual ~Renderer();

        const std::exception& Error() { return Job::GetCaughtException(); }
        bool                  IsErrored() const { return Job::IsFailed(); }
        bool                  IsRunning() const { return Job::IsExecuting(); }

        virtual void DrawAllMesh(ShaderProgram* pProgram);
        virtual void DrawAllSprites(ShaderProgram* pProgram);

        virtual GUI::IWindow* Window() const                                  = 0;
        virtual GUI::IWindow* Initialize(const GUI::WindowSize&, std::string) = 0;

        virtual VertexBuffer_ptr CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) = 0;
        virtual VertexBuffer_ptr CreateBuffer(
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces) = 0;

        virtual Shader_ptr CreateShader(
            const std::filesystem::path&              path,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) = 0;
        virtual Shader_ptr CreateShader(
            const std::filesystem::path&              path,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) = 0;

        virtual ShaderProgram_ptr CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) = 0;

        virtual Texture_ptr CreateTexture(const std::filesystem::path& path, TextureUnit unit) = 0;
        virtual Texture_ptr CreateTexture(const ImageFile& image, TextureUnit unit)            = 0;

        virtual void              ClearDepthBuffer() = 0;
        virtual void              ClearColorBuffer() = 0;
        virtual std::future<void> SwapBuffers()      = 0;

        void ExecuteFunction(std::function<void()> func);

      protected:
        void              ExecuteJob() final;
        std::future<void> ExecuteFunctionAsync(std::function<void()> func);

      private:
        std::pair<std::optional<std::promise<void>>, std::optional<std::function<void()>>> PopFunc();
        static void ExecuteFunction(std::promise<void>& prom, std::function<void()>& func);
        void        FlushQueue();
    };
} // namespace Ignosi::Modules::Render
