#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class CreateShaderProgram_Command : IRenderCommand
    {
      public:
        CreateShaderProgram_Command();
        ~CreateShaderProgram_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
