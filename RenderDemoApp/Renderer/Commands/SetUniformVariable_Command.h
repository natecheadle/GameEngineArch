#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class SetUniformVariable_Command : IRenderCommand
    {
      public:
        SetUniformVariable_Command();
        ~SetUniformVariable_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
