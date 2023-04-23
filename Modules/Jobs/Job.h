#pragma once

#include <chrono>
#include <condition_variable>
#include <exception>
#include <future>
#include <mutex>

namespace nate::Modules::Jobs
{

    class Job
    {
        std::future<void> m_Job;
        std::atomic<bool> m_ShouldStop{false};
        std::atomic<bool> m_ShouldPause{false};

        std::mutex              m_YieldMutex;
        std::condition_variable m_YieldCondition;

        std::atomic<bool> m_FailedJob{false};
        std::exception    m_ExitException;

      public:
        Job() = default;
        virtual ~Job();

        void                  Start();
        void                  Stop() { m_ShouldStop = true; }
        void                  Pause() { m_ShouldPause = true; }
        void                  Continue();
        bool                  IsFailed() const { return m_FailedJob; }
        const std::exception& GetCaughtException() const { return m_ExitException; }

        bool IsPaused() const { return m_ShouldPause; }
        bool IsExecuting() const;

        std::future_status Join(std::chrono::milliseconds timeout) const;
        void               Join() const;

      protected:
        virtual void ExecuteJob() = 0;
        void         Yield();
        bool         ShouldStop() const { return m_ShouldStop; }
    };
} // namespace nate::Modules::Jobs