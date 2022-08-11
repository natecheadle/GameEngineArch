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
            if (time_span > std::chrono::milliseconds(50))
            {
                std::cout << "time_span = " << double(time_span.count()) / 1e9 << " seconds \n";
            }

            UpdateApp(time_span);

            if (m_pRenderer->RenderingFailed())
            {
                std::cerr << m_pRenderer->GetFailure().what();
                return 1;
            }

            m_pRenderer->RenderFrame();
        }

        Shutdown();

        return 0;
    }

} // namespace nate::Modules::App