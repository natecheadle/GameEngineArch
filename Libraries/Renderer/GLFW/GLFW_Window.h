#pragma once

#include "IWindow.h"

#include <GLFW/glfw3.h>

#include <string>
#include <string_view>

namespace Ignosi::Libraries::Renderer
{

    class GLFW_Window : public IWindow
    {
        GLFWwindow* m_pWindow;
        std::string m_WindowName;

      public:
        GLFW_Window(std::string name, const WindowSize& size);
        ~GLFW_Window() override;

        const std::string& Name() const override;
        void               Name(std::string name) override;

        void*      NativeHandle() const override;
        WindowSize ActualWindowSize() const override;
        bool       ShouldClose() const override;
        void       SwapBuffers() const override;
        void       PollEvents() const override;

      private:
        static std::string_view LastError();
    };
} // namespace Ignosi::Libraries::Renderer
