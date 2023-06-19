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

        double period{0};
        while (!m_pWindow->ShouldClose() && m_pRenderer->IsRunning())
        {
            std::chrono::time_point<std::chrono::high_resolution_clock> begin =
                std::chrono::high_resolution_clock::now();
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

            m_pRenderer->SwapBuffers();

            std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
            period = std::chrono::duration<double>(end - begin).count();
        }

        Close();

        return 0;
    }

} // namespace nate::Modules::App