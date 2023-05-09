#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class CreateObject_Command : IRenderCommand
    {
      public:
        CreateObject_Command();
        ~CreateObject_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
