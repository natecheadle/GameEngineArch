#pragma once

#include "ILogBuffer.h"

#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/spdlog.h>

namespace nate::Modules::Debug::Logging
{

    class LogBuffer : public ILogBuffer
    {
        std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> m_Sink;

      public:
        LogBuffer(std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> sink)
            : m_Sink(std::move(sink))
        {
        }
        ~LogBuffer() = default;

        virtual std::vector<std::string> RetrieveAllLogs() { return m_Sink->last_formatted(); }
        virtual std::vector<std::string> RetrieveNLogs(size_t n) { return m_Sink->last_formatted(n); }

        std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> GetSink() const { return m_Sink; }
    };
} // namespace nate::Modules::Debug::Logging
