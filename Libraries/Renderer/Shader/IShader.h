#pragma once

#include <filesystem>
#include <string>

namespace Ignosi::Libraries::Renderer
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
        Compute,

        LAST
    };

    class IShader
    {
      public:
        IShader()          = default;
        virtual ~IShader() = default;

        virtual const std::string&           ShaderCode() const = 0;
        virtual const std::filesystem::path& ShaderLoc() const  = 0;
        virtual ShaderType                   Type() const       = 0;
    };
} // namespace Ignosi::Libraries::Renderer
