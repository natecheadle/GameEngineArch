#include "IWindow.h"

#include "GLFW/GLFW_Window.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include <memory>

namespace Ignosi::Libraries::Renderer
{

    std::ostream& operator<<(std::ostream& os, const WindowSize& value)
    {
        os << fmt::format("{}", value);
        return os;
    }

    std::string format_as(const WindowSize& value)
    {
        return fmt::format(".Width: {} .Height: {}", value.Width, value.Height);
    }

    std::unique_ptr<IWindow> IWindow::Create(std::string name, const WindowSize& size)
    {
        return std::make_unique<GLFW_Window>(std::move(name), size);
    }

} // namespace Ignosi::Libraries::Renderer
