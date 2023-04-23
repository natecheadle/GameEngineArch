#include "Job.h"

#include <exception>
#include <mutex>

namespace nate::Modules::Jobs
{
    Job::~Job()
    {
        std::unique_lock<std::mutex> lock(m_YieldMutex);
        m_ShouldStop = true;
        m_YieldCondition.notify_all();
        m_Job.wait();
    }

    void Job::Start()
    {
        if (!IsExecuting())
            m_Job = std::async([this]() {
                try
                {
                    ExecuteJob();
                }
                catch (const std::exception& e)
                {
                    m_ExitException = e;
                    m_FailedJob     = true;
                }
            });
    }

    void Job::Continue()
    {
        std::unique_lock<std::mutex> lock(m_YieldMutex);
        m_ShouldPause = false;
        m_YieldCondition.notify_all();
    }

    bool Job::IsExecuting() const
    {
        return m_Job.valid() && m_Job.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout;
    }

    std::future_status Job::Join(std::chrono::milliseconds timeout) const
    {
        return m_Job.wait_for(timeout);
    }

    void Job::Join() const
    {
        m_Job.wait();
    }

    void Job::Yield()
    {
        std::unique_lock<std::mutex> lock(m_YieldMutex);
        while (m_ShouldPause && !m_ShouldStop)
        {
            m_YieldCondition.wait_for(lock, std::chrono::milliseconds(10));
        }
    }
} // namespace nate::Modules::Jobs