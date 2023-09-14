#pragma once

#include <vector>

namespace Ignosi::Modules::Render
{
    class VertexBuffer
    {
      protected:
        VertexBuffer() = default;

      public:
        virtual ~VertexBuffer() = default;

        virtual void Draw() = 0;
    };
} // namespace Ignosi::Modules::Render
