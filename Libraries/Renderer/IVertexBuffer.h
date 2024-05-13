#pragma once

namespace Ignosi::Libraries::Renderer
{
    class IVertexBuffer
    {
      public:
        IVertexBuffer()          = default;
        virtual ~IVertexBuffer() = default;

        virtual void Draw() const = 0;
    };
} // namespace Ignosi::Libraries::Renderer
