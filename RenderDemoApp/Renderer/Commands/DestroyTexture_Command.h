#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class DestroyTexture_Command : IRenderCommand
    {
      public:
        DestroyTexture_Command();
        ~DestroyTexture_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
