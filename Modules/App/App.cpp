#include "App.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

namespace Ignosi::Modules::App
{

    App::App(Render::Renderer* pRenderer, const GUI::WindowSize& window_size, std::string window_name, Physics::PhysicsSystem* pPhysics)
        : m_pRenderer(pRenderer)
        , m_pWindow(pRenderer->InitializeWindow(window_size, std::move(window_name)))
        , m_pPhysics(pPhysics)
    {
    }

    void App::Close()
    {
        Shutdown();
    }

    int App::Run()
    {
        try
        {

            Initialize();

            double            period{0};
            std::future<void> swap_future;
            while (!m_pWindow->ShouldClose())
            {
                std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();
                m_pRenderer->ClearColorBuffer();
                m_pRenderer->ClearDepthBuffer();

                if (m_pWindow->ShouldClose())
                    break;

                UpdateApp(period);

                m_pRenderer->SwapBuffers();

                std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
                period                                                          = std::chrono::duration<double>(end - begin).count();
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "caught exception e " << e.what();
            // TODO there is probably other state managment that needs to happen here.
        }

        Close();

        return 0;
    }

} // namespace Ignosi::Modules::App