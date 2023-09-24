#include "App.h"

#include "IWorld.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

namespace Ignosi::Modules::App
{

    App::App(std::unique_ptr<ECS::IWorld> pWorld)
        : m_pWorld(std::move(pWorld))
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

            m_pWindow = Initialize();

            double period{0};
            while (!m_pWindow->ShouldClose())
            {
                const std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();

                if (m_pWindow->ShouldClose())
                    break;

                UpdateApp(period);
                m_pWorld->Update(period);

                const std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
                period                                                                = std::chrono::duration<double>(end - begin).count();
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
