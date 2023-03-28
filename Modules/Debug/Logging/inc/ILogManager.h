#pragma once

#include "ILogBuffer.h"
#include "ILogger.h"

#include <memory>
#include <string>
#include <vector>

namespace nate::Modules::Debug::Logging
{

    class ILogManager
    {
      public:
        ILogManager()          = default;
        virtual ~ILogManager() = default;

        static std::unique_ptr<ILogManager> Factory();

        virtual void EnableStdOutLogging()          = 0;
        virtual void DisableStdOutLogging()         = 0;
        virtual bool IsStdOutLoggingEnabled() const = 0;

        virtual void EnableDebugConsoleLogging()          = 0;
        virtual void DisableDebugConsoleLogging()         = 0;
        virtual bool IsDebugConsoleLoggingEnabled() const = 0;

        virtual void AttachFileLogging(const std::string& filePath)     = 0;
        virtual void DetachFileLogging(const std::string& filePath)     = 0;
        virtual bool IsFileLoggingAttached(const std::string& filePath) = 0;

        virtual ILogBuffer* AttachLogBuffer(size_t bufferSize)       = 0;
        virtual void        DetachLogBuffer(ILogBuffer* pLogger)     = 0;
        virtual bool        IsLogBufferAttached(ILogBuffer* pLogger) = 0;

        virtual std::vector<ILogger*> GetAllLoggers() const                           = 0;
        virtual ILogger*              InitializeLogger(const std::string& moduleName) = 0;
        virtual ILogger*              GetLogger(const std::string& moduleName) const  = 0;
    };
} // namespace nate::Modules::Debug::Logging
