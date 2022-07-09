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
        using ResizeFunc     = std::function<void(WindowSize)>;
        using CloseFunc      = std::function<void()>;
        using KeyPressFunc   = std::function<void(KeyState, KeyModifiers, int scanCode)>;
        using MouseClickFunc = std::function<void(MouseButton, KeyState, KeyModifiers)>;

        virtual ~IWindow() = default;

        virtual void AttachKeyCallback(Key key, KeyPressFunc callBack) = 0;
        virtual void ClearKeyCallback(Key key)                         = 0;

        virtual void AttachOnCloseCallback(CloseFunc callback) = 0;
        virtual void ClearOnCloseCallback()                    = 0;

        virtual void AttachOnResizeCallback(ResizeFunc callback) = 0;
        virtual void ClearOnResizeCallback()                     = 0;

        virtual void AttachMouseCallback(MouseClickFunc callback) = 0;
        virtual void ClearMouseCallback()                         = 0;

        virtual void PollEvents() const  = 0;
        virtual void Close() const       = 0;
        virtual bool ShouldClose() const = 0;

        virtual bool               IsValid() const      = 0;
        virtual std::string        LastError() const    = 0;
        virtual const std::string& Name() const         = 0;
        virtual void*              NativeHandle() const = 0;
        virtual WindowSize         InitialSize() const  = 0;

        virtual CursorPosition QueryCursorPosition() const  = 0;
        virtual KeyState       QueryKeyState(Key key) const = 0;
        virtual WindowSize     QueryWindowSize() const      = 0;

      private:
    };
} // namespace nate::Modules::GUI