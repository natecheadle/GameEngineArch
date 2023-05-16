#pragma once

#include <vector>

namespace nate::Modules::Render
{
    class VertexBuffer
    {
      protected:
        VertexBuffer() = default;

      public:
        virtual ~VertexBuffer() = default;

        virtual void Draw() = 0;
    };
} // namespace nate::Modules::Render
