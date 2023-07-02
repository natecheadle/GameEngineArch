#include "Logger.h"

#include "LogManager.h"

namespace Ignosi::Modules::Debug::Logging
{
    Logger::Logger(ILogManager* pManager, const std::string& moduleName, spdlog::sink_ptr sink)
        : m_pManager(pManager)
    {
        // This matches the defaults
        spdlog::init_thread_pool(8192, 1);
        m_ThreadPool = spdlog::thread_pool();
        m_Logger     = std::make_shared<spdlog::async_logger>(
            moduleName,
            sink,
            m_ThreadPool,
            spdlog::async_overflow_policy::block);
        m_Logger->set_level(spdlog::level::level_enum::trace);
    }

    const std::string& Logger::GetModuleName() const
    {
        return m_Logger->name();
    }

    void Logger::PushErrorLog(std::string_view value) const
    {
        m_Logger->error(value);
    }

    void Logger::PushWarningLog(std::string_view value) const
    {
        m_Logger->warn(value);
    }

    void Logger::PushDebugLog(std::string_view value) const
    {
        m_Logger->debug(value);
    }

    void Logger::PushTraceLog(std::string_view value) const
    {
        m_Logger->trace(value);
    }

    void Logger::PushLog(LogLevel level, std::string_view value) const
    {
        switch (level)
        {
        case LogLevel::Debug: PushDebugLog(value); break;
        case LogLevel::Trace: PushTraceLog(value); break;
        case LogLevel::Warning: PushWarningLog(value); break;
        case LogLevel::Error: PushErrorLog(value); break;
        default: break;
        }
    }

    void Logger::SetLogLevel(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug: m_Logger->set_level(spdlog::level::level_enum::debug); break;
        case LogLevel::Trace: m_Logger->set_level(spdlog::level::level_enum::trace); break;
        case LogLevel::Warning: m_Logger->set_level(spdlog::level::level_enum::warn); break;
        case LogLevel::Error: m_Logger->set_level(spdlog::level::level_enum::err); break;
        case LogLevel::Off: m_Logger->set_level(spdlog::level::level_enum::off); break;
        }
    }

    void Logger::Flush() const
    {
        m_Logger->flush();
    }

} // namespace Ignosi::Modules::Debug::Logging
