#pragma once

#include "CursorPosition.hpp"
#include "KeyModifiers.hpp"
#include "Keys.h"
#include "WindowSize.hpp"

#include <functional>
#include <memory>
#include <string>

namespace nate::Modules::GUI {

    class IWindow {
      public:
        virtual ~IWindow() = default;

        static std::unique_ptr<IWindow> Factory(int width, int height, std::string windowName = "");

        virtual void AttachKeyCallback(
            Key                                                         key,
            std::function<void(KeyState state, KeyModifiers modifiers)> callBack) = 0;
        virtual void ClearKeyCallback(Key key)                                    = 0;

        virtual void AttachOnCloseCallback(std::function<void()> callback) = 0;
        virtual void ClearOnCloseCallback()                                = 0;

        virtual void AttachOnResizeCallback(std::function<void(int width, int height)> callback) = 0;
        virtual void ClearOnResizeCallback()                                                     = 0;

        virtual void AttachMouseCallback(std::function<void(MouseButton button, KeyState state)> callback) = 0;
        virtual void ClearMouseCallback()                                                                  = 0;

        virtual void Close() const       = 0;
        virtual bool ShouldClose() const = 0;

        virtual bool               IsValid() const       = 0;
        virtual std::string        LastError() const     = 0;
        virtual const std::string& Name() const          = 0;
        virtual void*              NativeHandle() const  = 0;
        virtual int                InitialWidth() const  = 0;
        virtual int                InitialHeight() const = 0;

        virtual CursorPosition QueryCursorPosition() const  = 0;
        virtual KeyState       QueryKeyState(Key key) const = 0;
        virtual WindowSize     QueryWindowSize() const      = 0;

      private:
    };
} // namespace nate::Modules::GUI