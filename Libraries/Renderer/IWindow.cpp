#include "IWindow.h"

#include "GLFW_Window.h"

#include <memory>

namespace Ignosi::Libraries::Renderer
{

    std::ostream& operator<<(std::ostream& os, const WindowSize& value)
    {
        os << " .Width " << value.Width << " .Height " << value.Height;
        return os;
    }

    std::unique_ptr<IWindow> IWindow::Create(std::string name, const WindowSize& size)
    {
        return std::make_unique<GLFW_Window>(std::move(name), size);
    }

} // namespace Ignosi::Libraries::Renderer
