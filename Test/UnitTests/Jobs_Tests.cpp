#include <Job.h>

#include <gtest/gtest.h>

#include <chrono>
#include <numeric>
#include <thread>
#include <variant>

namespace nate::Test {

    class TestJob : public Modules::Jobs::Job {
        std::vector<int>::const_iterator m_CurLoc;
        std::vector<int>::const_iterator m_End;

        long long         m_Sum;
        std::atomic<bool> m_IsComplete;

      public:
        TestJob(const std::vector<int>& numbersToSum)
            : m_CurLoc(numbersToSum.begin())
            , m_End(numbersToSum.end())
            , m_Sum(0)
            , m_IsComplete(false)
        {
        }

        ~TestJob() override = default;

        long long GetResult()
        {
            if (m_IsComplete)
                return m_Sum;
            return 0;
        }

      protected:
        void ExecuteJob() override
        {
            for (auto it = m_CurLoc; it < m_End; ++it)
            {
                if (IsPaused())
                {
                    Yield();
                }
                if (ShouldStop())
                {
                    m_IsComplete = false;
                    m_Sum        = 0;
                    return;
                }

                m_Sum += *it;

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            m_IsComplete = true;
        }
    };

    const std::vector<int> Items{1, 2, 3, 4, 5, 6, 7, 8, 9};

    TEST(Jobs_Tests, ValidateBasicRun)
    {
        TestJob testJob(Items);

        testJob.Start();
        ASSERT_TRUE(testJob.IsExecuting());

        testJob.Join(std::chrono::milliseconds(1000));
        ASSERT_FALSE(testJob.IsExecuting());

        ASSERT_EQ(std::accumulate(Items.begin(), Items.end(), 0), testJob.GetResult());
    }

    TEST(Jobs_Tests, ValidateStartStop)
    {

        TestJob testJob(Items);

        testJob.Start();
        ASSERT_TRUE(testJob.IsExecuting());
        testJob.Stop();
        testJob.Join(std::chrono::milliseconds(1000));
        ASSERT_FALSE(testJob.IsExecuting());
        ASSERT_EQ(0, testJob.GetResult());
    }

    TEST(Jobs_Tests, ValidatePauseContinue)
    {

        TestJob testJob(Items);

        testJob.Start();
        ASSERT_TRUE(testJob.IsExecuting());
        testJob.Pause();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        ASSERT_TRUE(testJob.IsPaused());
        ASSERT_TRUE(testJob.IsExecuting());
        testJob.Continue();

        testJob.Join(std::chrono::milliseconds(1000));
        ASSERT_FALSE(testJob.IsExecuting());
        ASSERT_EQ(std::accumulate(Items.begin(), Items.end(), 0), testJob.GetResult());
    }

    TEST(Jobs_Tests, ValidateDestroyNotStopped)
    {
        TestJob testJob(Items);

        testJob.Start();
        ASSERT_TRUE(testJob.IsExecuting());
    }
} // namespace nate::Test