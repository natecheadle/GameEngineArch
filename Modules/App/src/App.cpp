#include "App.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

namespace nate::Modules::App
{

    App::App(std::unique_ptr<GUI::IWindow> pWindow, std::unique_ptr<Render::Renderer> pRenderer)
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
        while (!m_pRenderer->IsInitialized())
            std::this_thread::yield();

        Initialize();

        m_pRenderer->StartRendering();

        std::chrono::nanoseconds period{0};
        while (!m_pWindow->ShouldClose() && m_pRenderer->IsRunning())
        {
            std::chrono::time_point<std::chrono::steady_clock> begin = std::chrono::steady_clock::now();
            m_pWindow->PollEvents();
            if (m_pWindow->ShouldClose())
                break;

            UpdateApp(period);

            if (m_pRenderer->RenderingFailed())
            {
                std::cerr << m_pRenderer->GetFailure().what();
                return 1;
            }

            std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
            period                             = duration_cast<std::chrono::nanoseconds>(begin - end);
            std::chrono::nanoseconds sleepTime = std::chrono::nanoseconds(std::int64_t(1.0 / 60.0 * 1e9)) - period;
            std::this_thread::sleep_for(sleepTime);

            m_pRenderer->RenderFrame();
        }

        Close();

        return 0;
    }

} // namespace nate::Modules::App