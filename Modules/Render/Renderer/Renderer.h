#pragma once

#include "../3D/Light_Directional.h"
#include "../3D/Light_Point.h"
#include "../3D/Light_Spotlight.h"
#include "../3D/Material.h"
#include "../3D/Mesh3D.h"
#include "../3D/Model3D.h"
#include "../Shader/Shader.h"
#include "../Texture/Texture.h"
#include "3D/Model3D.h"
#include "Shader/ShaderProgram.h"
#include "VertexBuffer.h"

#include <IWindow.h>
#include <Job.h>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <exception>
#include <filesystem>
#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <queue>
#include <span>

namespace nate::Modules::Render
{
    using VertexBuffer_ptr  = std::unique_ptr<VertexBuffer, std::function<void(VertexBuffer*)>>;
    using Shader_ptr        = std::unique_ptr<Shader, std::function<void(Shader*)>>;
    using Texture_ptr       = std::unique_ptr<Texture, std::function<void(Texture*)>>;
    using ShaderProgram_ptr = std::unique_ptr<ShaderProgram, std::function<void(ShaderProgram*)>>;

    class Renderer : protected Jobs::Job
    {
        std::thread::id                                                  m_RenderThreadID;
        std::mutex                                                       m_QueueMutex;
        std::condition_variable                                          m_QueueCondition;
        std::queue<std::pair<std::promise<void>, std::function<void()>>> m_CommandQueue;

      public:
        virtual ~Renderer();

        static std::unique_ptr<Renderer> Create();

        const std::exception& Error() { return Job::GetCaughtException(); }
        bool                  IsErrored() const { return Job::IsFailed(); }
        bool                  IsRunning() const { return Job::IsExecuting(); }

        virtual GUI::IWindow* Window() const                                            = 0;
        virtual GUI::IWindow* Initialize(const GUI::WindowSize& size, std::string name) = 0;

        virtual VertexBuffer_ptr CreateBuffer(const VertexDataConfig& config, std::span<float> vertexes) = 0;
        virtual VertexBuffer_ptr CreateBuffer(
            const VertexDataConfig&  config,
            std::span<float>         vertexes,
            std::span<std::uint32_t> indeces) = 0;

        virtual Shader_ptr CreateShader(const std::filesystem::path& path)                  = 0;
        virtual Shader_ptr CreateShader(const std::filesystem::path& path, ShaderType type) = 0;

        virtual ShaderProgram_ptr CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) = 0;

        virtual Texture_ptr CreateTexture(const std::filesystem::path& path, TextureUnit unit) = 0;
        virtual Texture_ptr CreateTexture(const ImageFile& image, TextureUnit unit)            = 0;

        virtual void ClearDepthBuffer() = 0;
        virtual void ClearColorBuffer() = 0;
        virtual void SwapBuffers()      = 0;

        void ExecuteFunction(std::function<void()> func);

      protected:
        void ExecuteJob() final;

      private:
        std::future<void> ExecuteFunctionAsync(std::function<void()> func);
        std::pair<std::optional<std::promise<void>>, std::optional<std::function<void()>>> PopFunc();
        static void ExecuteFunction(std::promise<void>& prom, std::function<void()>& func);
        void        FlushQueue();
    };
} // namespace nate::Modules::Render
