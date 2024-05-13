#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace Ignosi::Libraries::Renderer
{

    struct WindowSize
    {
        int Width;
        int Height;

        float AspectRatio() { return static_cast<float>(Width) / static_cast<float>(Height); }

        friend bool          operator==(const WindowSize& lhs, const WindowSize& rhs) = default;
        friend std::ostream& operator<<(std::ostream& os, const WindowSize& value);
    };

    std::string format_as(const WindowSize& value);

    class IWindow
    {
      public:
        IWindow()          = default;
        virtual ~IWindow() = default;

        IWindow(const IWindow& other)     = delete;
        IWindow(IWindow&& other) noexcept = delete;

        IWindow& operator=(const IWindow& other)     = delete;
        IWindow& operator=(IWindow&& other) noexcept = delete;

        static std::unique_ptr<IWindow> Create(std::string name, const WindowSize& size = {.Width = 800, .Height = 600});

        virtual const std::string& Name() const           = 0;
        virtual void               Name(std::string name) = 0;

        virtual void*      NativeHandle() const     = 0;
        virtual WindowSize ActualWindowSize() const = 0;
        virtual bool       ShouldClose() const      = 0;
        virtual void       SwapBuffers() const      = 0;
        virtual void       PollEvents() const       = 0;
    };
} // namespace Ignosi::Libraries::Renderer
