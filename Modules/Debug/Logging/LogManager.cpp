#include "LogManager.h"

#include "LogBuffer.h"
#include "Logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace Ignosi::Modules::Debug::Logging
{
    LogManager::LogManager()
        : m_Sink(std::make_shared<spdlog::sinks::dist_sink_mt>())
    {
    }

    LogManager::~LogManager()
    {
        m_StdOutSink.reset();
#if defined(_WIN32)
        m_DebugSink.reset();
#endif
        m_FileSinks.clear();
        m_BufferSinks.clear();

        for (auto& logger : m_Loggers)
        {
            logger.second->Flush();
        }

        m_Loggers.clear();
        m_Sink.reset();

        spdlog::shutdown();
    }

    void LogManager::EnableStdOutLogging()
    {
        if (!m_StdOutSink)
        {
            m_StdOutSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
            m_Sink->add_sink(m_StdOutSink);
        }
    }

    void LogManager::DisableStdOutLogging()
    {
        if (m_StdOutSink)
        {
            m_Sink->remove_sink(m_StdOutSink);
            m_StdOutSink.reset();
        }
    }

    void LogManager::EnableDebugConsoleLogging()
    {
#if defined(_WIN32)
        if (!m_DebugSink)
        {
            m_DebugSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
            m_Sink->add_sink(m_DebugSink);
        }
#endif
    }

    void LogManager::DisableDebugConsoleLogging()
    {
#if defined(_WIN32)
        if (m_DebugSink)
        {
            m_Sink->remove_sink(m_DebugSink);
            m_DebugSink.reset();
        }
#endif
    }

    void LogManager::AttachFileLogging(const std::string& filePath)
    {
        if (!IsFileLoggingAttached(filePath))
        {
            auto newSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath);
            m_Sink->add_sink(newSink);
            m_FileSinks.insert({filePath, std::move(newSink)});
        }
    }

    void LogManager::DetachFileLogging(const std::string& filePath)
    {
        auto it = m_FileSinks.find(filePath);
        if (it == m_FileSinks.end())
            return;

        m_Sink->remove_sink(it->second);
        m_FileSinks.erase(it);
    }

    ILogBuffer* LogManager::AttachLogBuffer(size_t bufferSize)
    {
        auto newSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(bufferSize);
        m_BufferSinks.push_back(std::make_unique<LogBuffer>(newSink));
        m_Sink->add_sink(std::move(newSink));

        return m_BufferSinks.back().get();
    }

    void LogManager::DetachLogBuffer(ILogBuffer* pLogger)
    {
        auto it = GetBufferLogger(pLogger);
        if (it != m_BufferSinks.end())
        {
            m_Sink->remove_sink(it->get()->GetSink());
            m_BufferSinks.erase(it);
        }
    }

    bool LogManager::IsLogBufferAttached(ILogBuffer* pLogger)
    {
        return GetBufferLogger(pLogger) != m_BufferSinks.end();
    }

    std::vector<ILogger*> LogManager::GetAllLoggers() const
    {
        std::vector<ILogger*> allLoggers;
        allLoggers.reserve(m_Loggers.size());
        for (const auto& pair : m_Loggers)
        {
            allLoggers.push_back(pair.second.get());
        }

        return allLoggers;
    }

    ILogger* LogManager::InitializeLogger(const std::string& moduleName)
    {
        auto searchRslt = m_Loggers.find(moduleName);
        if (searchRslt == m_Loggers.end())
        {
            auto    newLogger = std::make_unique<Logger>(this, moduleName, m_Sink);
            Logger* returnVal = newLogger.get();
            m_Loggers.insert({moduleName, std::move(newLogger)});
            return returnVal;
        }

        return searchRslt->second.get();
    }

    ILogger* LogManager::GetLogger(const std::string& moduleName) const
    {
        auto searchRslt = m_Loggers.find(moduleName);

        if (searchRslt == m_Loggers.end())
        {
            return nullptr;
        }

        return searchRslt->second.get();
    }

    std::list<std::unique_ptr<LogBuffer>>::iterator LogManager::GetBufferLogger(ILogBuffer* pLogger)
    {
        for (auto it = m_BufferSinks.begin(); it != m_BufferSinks.end(); ++it)
        {
            if (static_cast<ILogBuffer*>(it->get()) == pLogger)
            {
                return it;
            }
        }
        return m_BufferSinks.end();
    }
} // namespace Ignosi::Modules::Debug::Logging
