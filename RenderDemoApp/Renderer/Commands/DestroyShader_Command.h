#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class DestroyShader_Command : IRenderCommand
    {
      public:
        DestroyShader_Command();
        ~DestroyShader_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
