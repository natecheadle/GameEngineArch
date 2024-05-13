#include "IRenderer.h"

#include "OpenGL/OpenGL_Renderer.h"

namespace Ignosi::Libraries::Renderer
{
    std::unique_ptr<IRenderer> IRenderer::Create()
    {
        return std::make_unique<OpenGL_Renderer>();
    }
} // namespace Ignosi::Libraries::Renderer
