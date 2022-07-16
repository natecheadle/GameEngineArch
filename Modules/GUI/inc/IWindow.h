#pragma once

#include "CursorPosition.hpp"
#include "KeyModifiers.hpp"
#include "Keys.h"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <Message.hpp>

#include <functional>
#include <memory>
#include <string>

namespace nate::Modules::GUI {

    class IWindow {
      public:
        IWindow()          = default;
        virtual ~IWindow() = default;

        virtual void SubscribeToMessage(
            void*                                                          subscriber,
            WindowMessages                                                 id,
            std::function<void(const Messaging::Message<WindowMessages>*)> callback) = 0;

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