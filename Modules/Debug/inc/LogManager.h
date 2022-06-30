#pragma once
#include "ILogManager.h"
#include "LogBuffer.h"
#include "Logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace amc::Shared {
    class LogManager : public ILogManager {
        std::map<std::string, std::unique_ptr<Logger>> m_Loggers;
        std::shared_ptr<spdlog::sinks::dist_sink_mt>   m_Sink;

        std::shared_ptr<spdlog::sinks::stdout_sink_mt>                            m_StdOutSink;
        std::shared_ptr<spdlog::sinks::msvc_sink_mt>                              m_DebugSink;
        std::map<std::string, std::shared_ptr<spdlog::sinks::basic_file_sink_mt>> m_FileSinks;
        std::list<std::unique_ptr<LogBuffer>>                                     m_BufferSinks;

      public:
        LogManager();
        ~LogManager();

        void EnableStdOutLogging() override;
        void DisableStdOutLogging() override;
        bool IsStdOutLoggingEnabled() const override { return m_StdOutSink != nullptr; }

        void EnableDebugConsoleLogging() override;
        void DisableDebugConsoleLogging() override;
        bool IsDebugConsoleLoggingEnabled() const override { return m_DebugSink != nullptr; }

        void AttachFileLogging(const std::string& filePath) override;
        void DetachFileLogging(const std::string& filePath) override;
        bool IsFileLoggingAttached(const std::string& filePath) override
        {
            return m_FileSinks.find(filePath) != m_FileSinks.end();
        }

        ILogBuffer* AttachLogBuffer(size_t bufferSize) override;
        void        DetachLogBuffer(ILogBuffer* pLogger) override;
        bool        IsLogBufferAttached(ILogBuffer* pLogger) override;

        std::vector<ILogger*> GetAllLoggers() const override;
        ILogger*              InitializeLogger(const std::string& moduleName) override;
        ILogger*              GetLogger(const std::string& moduleName) const override;

      private:
        std::list<std::unique_ptr<LogBuffer>>::iterator GetBufferLogger(ILogBuffer* pLogger);
    };
} // namespace amc::Shared
