#pragma once

#include "../3D/Object3D.h"
#include "../Shader/Shader.h"
#include "../Texture/Texture.h"

#include <Job.h>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <filesystem>
#include <future>
#include <memory>
#include <optional>
#include <queue>


namespace nate::Modules::Render
{
    class Renderer : public Jobs::Job
    {
        static std::unique_ptr<Renderer> s_pInstance;

        std::mutex                                                       m_QueueMutex;
        std::condition_variable                                          m_QueueCondition;
        std::queue<std::pair<std::promise<void>, std::function<void()>>> m_CommandQueue;

      public:
        virtual ~Renderer();

        static void      SetInstance(std::unique_ptr<Renderer> pRenderer) { s_pInstance = std::move(pRenderer); }
        static Renderer* GetInstance()
        {
            assert(s_pInstance.get());
            return s_pInstance.get();
        }

        virtual std::unique_ptr<Object3D> CreateObject(std::vector<VertexData> vertexes) = 0;
        virtual std::unique_ptr<Object3D> CreateObject(
            std::vector<VertexData>    vertexes,
            std::vector<std::uint32_t> indeces) = 0;

        virtual std::unique_ptr<Shader> CreateShader(const std::filesystem::path& path)                  = 0;
        virtual std::unique_ptr<Shader> CreateShader(const std::filesystem::path& path, ShaderType type) = 0;

        virtual std::unique_ptr<ShaderProgram> CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) = 0;

        virtual std::unique_ptr<Texture> CreateTexture(const std::filesystem::path& path, TextureUnit unit) = 0;
        virtual std::unique_ptr<Texture> CreateTexture(const ImageFile& image, TextureUnit unit)            = 0;

        virtual void Draw(Object3D* pObj) = 0;

        virtual void SetShaderVar(ShaderProgram* pShader, const std::string& name, bool value)  = 0;
        virtual void SetShaderVar(ShaderProgram* pShader, const std::string& name, int value)   = 0;
        virtual void SetShaderVar(ShaderProgram* pShader, const std::string& name, float value) = 0;
        virtual void SetShaderVar(
            ShaderProgram*                pShader,
            const std::string&            name,
            const SquareMatrix<4, float>& value)                                                                  = 0;
        virtual void SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<3, float>& value) = 0;

      protected:
        void              ExecuteJob() final;
        void              ExecuteFunction(std::function<void()> func);
        std::future<void> ExecuteFunctionAsync(std::function<void()> func);

      private:
        std::pair<std::optional<std::promise<void>>, std::optional<std::function<void()>>> PopFunc();

        virtual void DestroyObject(Object3D* pObj)                 = 0;
        virtual void DestroyShader(Shader* pShader)                = 0;
        virtual void DestroyTexture(Texture* pTex)                 = 0;
        virtual void DestroyShaderProgram(ShaderProgram* pProgram) = 0;
    };
} // namespace nate::Modules::Render
