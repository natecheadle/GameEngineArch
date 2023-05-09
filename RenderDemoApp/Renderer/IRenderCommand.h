#pragma once

namespace nate::Modules::Render
{
    enum class RenderCommandType
    {
        CreateObject,
        DestroyObject,
        CreateTexture,
        DestroyTexture,
        CreateShader,
        DestroyShader,
        CreateShaderProgram,
        DestroyShaderProgram,
        SetUniformVariable,
        DrawObject,
    };

    class IRenderCommand
    {
      public:
        virtual ~IRenderCommand()              = default;
        virtual void              Execute()    = 0;
        virtual RenderCommandType Type() const = 0;
    };
} // namespace nate::Modules::Render
