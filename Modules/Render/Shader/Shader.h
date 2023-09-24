#pragma once

#include "Resource.h"

#include <filesystem>
#include <string>

namespace Ignosi::Modules::Render
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
        Compute,

        LAST
    };

    class Shader : public ECS::Resource
    {
      private:
        std::filesystem::path m_ShaderLoc;
        std::string           m_ShaderCode;

      protected:
        Shader(const std::filesystem::path& shaderLoc);

      public:
        virtual ~Shader() = default;

        const std::string&           ShaderCode() const { return m_ShaderCode; }
        const std::filesystem::path& ShaderLoc() const { return m_ShaderLoc; }
        virtual ShaderType           Type() const = 0;
    };
} // namespace Ignosi::Modules::Render
