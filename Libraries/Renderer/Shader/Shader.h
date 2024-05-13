#pragma once

#include "IShader.h"

#include <filesystem>
#include <string>

namespace Ignosi::Libraries::Renderer
{
    class Shader : public IShader
    {
      private:
        std::filesystem::path m_ShaderLoc;
        std::string           m_ShaderCode;

      protected:
        Shader(const std::filesystem::path& shaderLoc);

      public:
        virtual ~Shader() = default;

        const std::string&           ShaderCode() const override { return m_ShaderCode; }
        const std::filesystem::path& ShaderLoc() const override { return m_ShaderLoc; }
    };
} // namespace Ignosi::Libraries::Renderer
