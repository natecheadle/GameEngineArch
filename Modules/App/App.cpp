#include "App.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

namespace nate::Modules::App
{

    App::App(std::unique_ptr<Render::Renderer> pRenderer, const GUI::WindowSize& window_size, std::string window_name)
        : m_pRenderer(std::move(pRenderer))
    {
        m_pWindow = m_pRenderer->Initialize(window_size, std::move(window_name));
    }

    void App::Close()
    {
        Shutdown();
        m_pRenderer.reset();
    }

    int App::Run()
    {
        Initialize();

        std::chrono::nanoseconds period{0};
        while (!m_pWindow->ShouldClose() && m_pRenderer->IsRunning())
        {
            std::chrono::time_point<std::chrono::steady_clock> begin = std::chrono::steady_clock::now();
            m_pRenderer->ClearColorBuffer();
            m_pRenderer->ClearDepthBuffer();

            if (m_pWindow->ShouldClose())
                break;

            UpdateApp(period);

            if (m_pRenderer->IsErrored())
            {
                std::cerr << m_pRenderer->Error().what();
                return 1;
            }

            std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
            period                             = duration_cast<std::chrono::nanoseconds>(begin - end);
            std::chrono::nanoseconds sleepTime = std::chrono::nanoseconds(std::int64_t(1.0 / 60.0 * 1e9)) - period;
            std::this_thread::sleep_for(sleepTime);

            m_pRenderer->SwapBuffers();
            m_pWindow->PollEvents();
        }

        Close();

        return 0;
    }

} // namespace nate::Modules::App