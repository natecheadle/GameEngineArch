#include <ILogBuffer.h>
#include <ILogManager.h>
#include <ILogger.h>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <thread>

using namespace nate::Modules::Debug;

namespace nate::Test {

    class Logging_Tests : public testing::Test {
      protected:
        std::unique_ptr<Logging::ILogManager> logManager{nullptr};
        Logging::ILogger*                     pLogger{nullptr};
        std::string                           LoggerName{"TestLogger"};

        Logging_Tests() {}

        void SetUp()
        {
            ASSERT_NO_THROW(logManager = Logging::ILogManager::Factory());
            ASSERT_NO_THROW(pLogger = logManager->InitializeLogger(LoggerName));
            ASSERT_NE(nullptr, pLogger);

            ASSERT_NO_THROW(pLogger->SetLogLevel(Logging::LogLevel::Trace));
        }
    };

    TEST_F(Logging_Tests, ValidateConstruction) {}

    TEST_F(Logging_Tests, ValidateGetLogger)
    {
        Logging::ILogger* pLogger2{nullptr};
        ASSERT_NO_THROW(pLogger2 = logManager->GetLogger(LoggerName));
        ASSERT_EQ(pLogger, pLogger2);

        ASSERT_NO_THROW(pLogger2 = logManager->GetLogger("NotTestLogger"));
        ASSERT_EQ(nullptr, pLogger2);
    }

    TEST_F(Logging_Tests, ValidateStdOutLogging)
    {
        ASSERT_FALSE(logManager->IsStdOutLoggingEnabled());
        ASSERT_NO_THROW(logManager->EnableStdOutLogging());
        ASSERT_TRUE(logManager->IsStdOutLoggingEnabled());

        std::string trace("This is a debug log");
        ASSERT_NO_THROW(pLogger->PushDebugLog(trace));

        ASSERT_NO_THROW(logManager->DisableStdOutLogging());
        ASSERT_FALSE(logManager->IsStdOutLoggingEnabled());
    }

#ifdef WIN32
    TEST_F(Logging_Tests, ValidateDebugConsoleLogging)
    {
        ASSERT_FALSE(logManager->IsDebugConsoleLoggingEnabled());
        ASSERT_NO_THROW(logManager->EnableDebugConsoleLogging());
        ASSERT_TRUE(logManager->IsDebugConsoleLoggingEnabled());

        std::string trace("This is a debug log");
        ASSERT_NO_THROW(pLogger->PushDebugLog(trace));

        ASSERT_NO_THROW(logManager->DisableDebugConsoleLogging());
        ASSERT_FALSE(logManager->IsDebugConsoleLoggingEnabled());
    }
#endif

    TEST_F(Logging_Tests, ValidateFileLogging)
    {
        std::string logFile("TestLog.log");
        std::string trace("This is a debug log");

        {
            ASSERT_NO_THROW(std::filesystem::remove_all(logFile));

            ASSERT_FALSE(logManager->IsFileLoggingAttached(logFile));
            ASSERT_NO_THROW(logManager->AttachFileLogging(logFile));
            ASSERT_TRUE(logManager->IsFileLoggingAttached(logFile));

            ASSERT_NO_THROW(pLogger->PushDebugLog(trace));
            // Need to let logs propogate
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ASSERT_NO_THROW(pLogger->Flush());

            ASSERT_NO_THROW(logManager->DetachFileLogging(logFile));
            ASSERT_FALSE(logManager->IsFileLoggingAttached(logFile));

            ASSERT_NO_THROW(logManager->DetachFileLogging(logFile));
        }

        std::ifstream file;
        int           time{0};
        const int     timeout{5};

        while (!file.is_open() && time++ < timeout)
        {
            file.open(logFile);
            if (!file.is_open())
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        ASSERT_TRUE(file.is_open());
        std::string fileLine;
        std::getline(file, fileLine);
        ASSERT_TRUE(fileLine.find(trace) != std::string::npos);
        file.close();
        ASSERT_FALSE(file.is_open());

        ASSERT_LE(0, std::filesystem::remove_all(logFile));
    }

    TEST_F(Logging_Tests, ValidateBufferLogging)
    {
        std::string          trace("This is a debug log");
        Logging::ILogBuffer* pBufferLogger{nullptr};

        ASSERT_FALSE(logManager->IsLogBufferAttached(pBufferLogger));
        ASSERT_NO_THROW(pBufferLogger = logManager->AttachLogBuffer(16));
        ASSERT_TRUE(logManager->IsLogBufferAttached(pBufferLogger));

        ASSERT_NO_THROW(pLogger->PushErrorLog(trace));
        // Need to let logs propogate
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ASSERT_NO_THROW(pLogger->Flush());

        std::vector<std::string> logs = pBufferLogger->RetrieveAllLogs();
        ASSERT_LT(0U, logs.size());
        ASSERT_TRUE(logs[0].find(trace) != std::string::npos);

        ASSERT_NO_THROW(logManager->DetachLogBuffer(pBufferLogger));
        ASSERT_FALSE(logManager->IsLogBufferAttached(pBufferLogger));
    }

    TEST_F(Logging_Tests, ValidateUpdateSinkWhileLogging)
    {
        Logging::ILogBuffer* pBufferLogger1{nullptr};
        Logging::ILogBuffer* pBufferLogger2{nullptr};
        std::string          trace("This is a debug log");

        ASSERT_NO_THROW(pBufferLogger1 = logManager->AttachLogBuffer(128));

        std::atomic<bool> stopExecuting{false};
        auto              logMessages = [&]() {
            while (!stopExecuting)
            {
                pLogger->PushDebugLog(trace);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };

        std::thread loggingThread(logMessages);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        ASSERT_NO_THROW(pBufferLogger2 = logManager->AttachLogBuffer(128));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        stopExecuting = true;
        loggingThread.join();

        ASSERT_LT(0U, pBufferLogger1->RetrieveAllLogs().size());
        ASSERT_LT(0U, pBufferLogger2->RetrieveAllLogs().size());
    }

    TEST_F(Logging_Tests, ValidateLogLevels)
    {
        for (int i = (int)Logging::LogLevel::Error; i <= (int)Logging::LogLevel::Off; ++i)
        {
            std::vector<std::string> logs;
            std::string              trace("This is a debug log");
            {
                Logging::ILogBuffer* pBufferLogger{nullptr};

                ASSERT_NO_THROW(pBufferLogger = logManager->AttachLogBuffer(16));

                ASSERT_NO_THROW(pLogger->SetLogLevel((Logging::LogLevel)i));
                for (int j = (int)Logging::LogLevel::Error; j < (int)Logging::LogLevel::Off; ++j)
                {
                    ASSERT_NO_THROW(pLogger->PushLog((Logging::LogLevel)j, trace));
                }

                pLogger->PushDebugLog(trace);
                pLogger->PushErrorLog(trace);
                pLogger->PushTraceLog(trace);
                pLogger->PushWarningLog(trace);

                // Need to let logs propogate
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                pLogger->Flush();

                logs = pBufferLogger->RetrieveAllLogs();

                ASSERT_NO_THROW(logManager->DetachLogBuffer(pBufferLogger));
            }

            switch (i)
            {
            case (int)Logging::LogLevel::Debug: ASSERT_EQ(6, logs.size()); break;
            case (int)Logging::LogLevel::Trace: ASSERT_EQ(8, logs.size()); break;
            case (int)Logging::LogLevel::Warning: ASSERT_EQ(4, logs.size()); break;
            case (int)Logging::LogLevel::Error: ASSERT_EQ(2, logs.size()); break;
            case (int)Logging::LogLevel::Off: ASSERT_EQ(0, logs.size()); break;
            }
        }
    }
} // namespace nate::Test
