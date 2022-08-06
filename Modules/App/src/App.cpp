#include "App.h"

#include <cassert>
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

        while (!m_pWindow->ShouldClose() && m_pRenderer->IsRunning())
        {
            m_pWindow->PollEvents();
            if (m_pWindow->ShouldClose())
                break;

            UpdateApp();

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