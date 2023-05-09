#pragma once

#include "../IRenderCommand.h"

namespace nate::Modules::Render
{
    class DestroyObject_Command : IRenderCommand
    {
      public:
        DestroyObject_Command();
        ~DestroyObject_Command() override = default;
        void              Execute() override;
        RenderCommandType Type() const override;
    };
} // namespace nate::Modules::Render
