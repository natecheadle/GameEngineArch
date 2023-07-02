#pragma once

#include "../3D/Mesh3D.h"
#include "Renderer.h"
#include "Renderer/VertexBuffer.h"

#include <Window_GLFW.h>

#include <memory>

namespace Ignosi::Modules::Render
{

    class Renderer_OpenGL : public Renderer
    {
        std::unique_ptr<GUI::Window_GLFW> m_pWin;

      public:
        Renderer_OpenGL(Memory::PoolMemoryBlock<Mesh3D>* pMeshPool, Memory::PoolMemoryBlock<Sprite>* pSpritePool);

        ~Renderer_OpenGL() override;

        GUI::IWindow* Window() const override { return m_pWin.get(); }

        GUI::IWindow* Initialize(const GUI::WindowSize& size, std::string name) override;

        VertexBuffer_ptr CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) override;
        VertexBuffer_ptr CreateBuffer(
            const VertexDataConfig&        config,
            std::span<const float>         vertexes,
            std::span<const std::uint32_t> indeces) override;

        Shader_ptr CreateShader(
            const std::filesystem::path&              path,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) final;
        Shader_ptr CreateShader(
            const std::filesystem::path&              path,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>()) final;

        ShaderProgram_ptr CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) final;

        Texture_ptr CreateTexture(const std::filesystem::path& path, TextureUnit unit) final;
        Texture_ptr CreateTexture(const ImageFile& image, TextureUnit unit) final;

        void ClearDepthBuffer() override;
        void ClearColorBuffer() override;
        std::future<void> SwapBuffers() override;

      private:
        static bool Validate(void* pVoid);

        void OnWindowResized(const GUI::WindowMessage* pMessage);

        void Destroy(VertexBuffer* pObj);
        void Destroy(Shader* pShader);
        void Destroy(Texture* pTex);
        void Destroy(ShaderProgram* pProgram);

        template <class T>
        void SetShaderVar_T(ShaderProgram* pShader, const std::string& name, const T& value)
        {
            assert(pShader);
            ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
        }

        template <class T>
        void DeleteObj(T* pObj)
        {
            if (Validate(pObj))
            {
                ExecuteFunction([pObj]() { delete pObj; });
            }
        }
    };
} // namespace Ignosi::Modules::Render