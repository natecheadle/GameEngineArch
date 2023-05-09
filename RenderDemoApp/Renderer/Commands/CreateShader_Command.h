#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class CreateShader_Command : IRenderCommand
    {
      public:
        CreateShader_Command();
        ~CreateShader_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
