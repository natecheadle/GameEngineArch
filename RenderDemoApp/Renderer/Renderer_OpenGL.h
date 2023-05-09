#pragma once;

#include "Renderer.h"

namespace nate::Modules::Render
{
    class Renderer_OpenGL : public Renderer
    {
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
    };
} // namespace nate::Modules::Render