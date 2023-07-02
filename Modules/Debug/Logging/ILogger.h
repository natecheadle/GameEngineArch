#pragma once

#include <string_view>

namespace Ignosi::Modules::Debug::Logging
{

    class ILogManager;

    enum class LogLevel
    {
        Error   = 0,
        Warning = 1,
        Debug   = 2,
        Trace   = 3,
        Off     = 4,
    };

    class ILogger
    {
      public:
        virtual const std::string& GetModuleName() const = 0;

        virtual void SetLogLevel(LogLevel level) = 0;
        virtual void Flush() const               = 0;

        virtual void PushErrorLog(std::string_view value) const   = 0;
        virtual void PushWarningLog(std::string_view value) const = 0;
        virtual void PushDebugLog(std::string_view value) const   = 0;
        virtual void PushTraceLog(std::string_view value) const   = 0;

        virtual void PushLog(LogLevel level, std::string_view value) const = 0;

        virtual ILogManager* GetManager() const = 0;

      protected:
        ILogger()  = default;
        ~ILogger() = default;
    };
} // namespace Ignosi::Modules::Debug::Logging
