#include "App.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

namespace nate::Modules::App {

    App::App(std::unique_ptr<GUI::IWindow> pWindow, std::unique_ptr<Render::IRenderer> pRenderer)
        : m_pWindow(std::move(pWindow))
        , m_pRenderer(std::move(pRenderer))
    {
    }

    void App::Close()
    {
        m_pRenderer->Stop();
        while (!m_pRenderer->WaitingForShutdown())
        {
            m_pRenderer->RenderFrame();
            std::this_thread::yield();
        }
        Shutdown();
        m_pRenderer->Shutdown();
        m_pWindow->Close();
    }

    int App::Run()
    {
        while (!m_pRenderer->IsInitialized() && m_pRenderer->IsRunning())
        {
            std::this_thread::yield();
            m_pRenderer->RenderFrame();
        }

        Initialize();

        std::chrono::time_point<std::chrono::steady_clock> timePoint1 = std::chrono::steady_clock::now();
        std::chrono::time_point<std::chrono::steady_clock> timePoint2 = std::chrono::steady_clock::now();
        while (!m_pWindow->ShouldClose() && m_pRenderer->IsRunning())
        {
            m_pWindow->PollEvents();
            if (m_pWindow->ShouldClose())
                break;

            timePoint2     = std::chrono::steady_clock::now();
            auto time_span = duration_cast<std::chrono::nanoseconds>(timePoint2 - timePoint1);
            timePoint1     = timePoint2;

            UpdateApp(time_span);

            if (m_pRenderer->RenderingFailed())
            {
                std::cerr << m_pRenderer->GetFailure().what();
                return 1;
            }

            m_pRenderer->RenderFrame();
        }
        m_pRenderer->Stop();
        while (!m_pRenderer->WaitingForShutdown())
        {
            m_pRenderer->RenderFrame();
            std::this_thread::yield();
        }

        Shutdown();

        return 0;
    }

} // namespace nate::Modules::App