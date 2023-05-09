#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class DestroyShaderProgram_Command : IRenderCommand
    {
      public:
        DestroyShaderProgram_Command();
        ~DestroyShaderProgram_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
