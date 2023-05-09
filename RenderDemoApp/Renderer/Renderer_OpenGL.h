#pragma once

#include "Renderer.h"

namespace nate::Modules::Render
{
    class Renderer_OpenGL : public Renderer
    {
      public:
        ~Renderer_OpenGL() override;

        std::unique_ptr<Object3D> CreateObject(std::vector<VertexData> vertexes) final;
        std::unique_ptr<Object3D> CreateObject(std::vector<VertexData> vertexes, std::vector<std::uint32_t> indeces)
            final;

        std::unique_ptr<Shader> CreateShader(const std::filesystem::path& path) final;
        std::unique_ptr<Shader> CreateShader(const std::filesystem::path& path, ShaderType type) final;

        std::unique_ptr<ShaderProgram> CreateShaderProgram(
            const Shader* pFragmentShader,
            const Shader* pGeometryShader,
            const Shader* pVertexShader) final;

        std::unique_ptr<Texture> CreateTexture(const std::filesystem::path& path, TextureUnit unit) final;
        std::unique_ptr<Texture> CreateTexture(const ImageFile& image, TextureUnit unit) final;

        void Draw(Object3D* pObj) final;

        void SetShaderVar(ShaderProgram* pShader, const std::string& name, bool value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, int value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, float value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const SquareMatrix<4, float>& value) final;
        void SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<3, float>& value) final;
    };
} // namespace nate::Modules::Render