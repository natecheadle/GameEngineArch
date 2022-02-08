#pragma once

#include "spdlog/spdlog.h"

#include <filesystem>

namespace nate::Modules::Debug {
    class Logger {
      public:
        enum class ConsoleLogType
        {
            StdOutConsole,
            StdErrConsole,
            WinDebug,
        };

        Logger(ConsoleLogType consoleLogType);
        Logger(ConsoleLogType consoleLogType, const std::filesystem::path& logFilePath);
        Logger(const std::filesystem::path& logFilePath);

        ~Logger();

      private:
        spdlog::logger m_Logger;
    };
} // namespace nate::Modules::Debug
