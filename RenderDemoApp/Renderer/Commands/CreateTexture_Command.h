#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class CreateTexture_Command : IRenderCommand
    {
      public:
        CreateTexture_Command();
        ~CreateTexture_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
