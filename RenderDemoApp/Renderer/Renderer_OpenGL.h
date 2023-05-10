#pragma once

#include "../3D/Object3D.h"
#include "Renderer.h"

#include <memory>

namespace nate::Modules::Render
{

    class Renderer_OpenGL : public Renderer
    {
      public:
        Renderer_OpenGL();
        ~Renderer_OpenGL() override;

        std::shared_ptr<Object3D> CreateObject(std::vector<VertexData> vertexes) final;
        std::shared_ptr<Object3D> CreateObject(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces)
            final;

        std::shared_ptr<Shader> CreateShader(const std::filesystem::path& path) final;
        std::shared_ptr<Shader> CreateShader(const std::filesystem::path& path, ShaderType type) final;

        std::shared_ptr<ShaderProgram> CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) final;

        std::shared_ptr<Texture> CreateTexture(const std::filesystem::path& path, TextureUnit unit) final;
        std::shared_ptr<Texture> CreateTexture(const ImageFile& image, TextureUnit unit) final;

        void Draw(Object3D* pObj) final;

        void SetShaderVar(ShaderProgram* pShader, const std::string& name, bool value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, int value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, float value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const SquareMatrix<4, float>& value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<3, float>& value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<4, float>& value) final;

        void ClearDepthBuffer() override;
        void ClearColorBuffer() override;
        void SwapBuffers(GUI::IWindow* pWindow) override;

      private:
        void Destroy(Object3D* pObj);
        void Destroy(Shader* pShader);
        void Destroy(Texture* pTex);
        void Destroy(ShaderProgram* pProgram);
    };
} // namespace nate::Modules::Render