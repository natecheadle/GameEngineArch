
#include "IWindow.h"

#include "GLFWWindow.h"
namespace nate::Modules::GUI {

    std::unique_ptr<IWindow> IWindow::Factory(int width, int height, std::string windowName)
    {
        std::unique_ptr<IWindow> pWindow = std::make_unique<GLFWWindow>(width, height, std::move(windowName));
        return std::move(pWindow);
    }
} // namespace nate::Modules::GUI