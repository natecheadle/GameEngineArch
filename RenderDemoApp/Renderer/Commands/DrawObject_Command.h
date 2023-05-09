#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class DrawObject_Command : IRenderCommand
    {
      public:
        DrawObject_Command();
        ~DrawObject_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
