#pragma once

#include "ILogger.h"

#include <spdlog/async.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

#include <map>
#include <memory>
#include <vector>

namespace nate::Modules::Debug::Logging
{

    class ILogManager;

    class Logger : public ILogger
    {
        std::shared_ptr<spdlog::async_logger>         m_Logger;
        ILogManager*                                  m_pManager;
        std::shared_ptr<spdlog::details::thread_pool> m_ThreadPool;

      public:
        Logger(ILogManager* pManager, const std::string& moduleName, spdlog::sink_ptr sink);

        ~Logger() = default;

        const std::string& GetModuleName() const override;

        void SetLogLevel(LogLevel level) override;
        void Flush() const override;

        void PushErrorLog(std::string_view value) const override;
        void PushWarningLog(std::string_view value) const override;
        void PushDebugLog(std::string_view value) const override;
        void PushTraceLog(std::string_view value) const override;

        void PushLog(LogLevel level, std::string_view value) const override;

        ILogManager* GetManager() const override { return m_pManager; }
    };
} // namespace nate::Modules::Debug::Logging
