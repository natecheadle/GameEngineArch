#include "GLFW_Window.h"
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <stdexcept>

#if __linux__
#define GLFW_EXPOSE_NATIVE_X11
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#elif WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

namespace Ignosi::Libraries::Renderer
{

GLFW_Window::GLFW_Window(std::string name, const WindowSize &size) : m_pWindow(nullptr), m_WindowName(std::move(name))
{

    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error(fmt::format("Failed to create window, received error {}", LastError()));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_pWindow = glfwCreateWindow(size.Width, size.Height, m_WindowName.c_str(), nullptr, nullptr);

    if (!m_pWindow)
        throw std::runtime_error(fmt::format("Failed to create window, received error {}", LastError()));

    glfwMakeContextCurrent(m_pWindow);
}

GLFW_Window::~GLFW_Window()
{
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

const std::string &GLFW_Window::Name() const
{
    return m_WindowName;
}

void GLFW_Window::Name(std::string name)
{
    m_WindowName = std::move(name);
    glfwSetWindowTitle(m_pWindow, m_WindowName.c_str());
}

WindowSize GLFW_Window::ActualWindowSize() const
{
    int width{0};
    int height{0};
    glfwGetWindowSize(m_pWindow, &width, &height);

    return {
        .Width = width,
        .Height = height,
    };
}

void *GLFW_Window::NativeHandle() const
{

#if __linux__
    return static_cast<void *>(glfwGetX11Window(m_pWindow));
#elif __APPLE__
    return static_cast<void *>(glfwGetCocoaWindow(m_pWindow));
#elif WIN32
    return static_cast<void *>(glfwGetWin32Window(m_pWindow));
#else
    static_assert(false, "Invalid native environment.");
#endif
}

bool GLFW_Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_pWindow) == GLFW_TRUE;
}

void GLFW_Window::PollEvents() const
{
    glfwPollEvents();
}

void GLFW_Window::SwapBuffers() const
{
    glfwSwapBuffers(m_pWindow);
}

std::string_view GLFW_Window::LastError()
{
    const char *error{nullptr};
    glfwGetError(&error);
    return error;
}
} // namespace Ignosi::Libraries::Renderer
