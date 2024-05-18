#include "App.h"

namespace Ignosi::Libraries::ECS
{
    void App::Run()
    {
        Initialize();

        while (KeepUpdating())
        {
            Update();
        }

        ShutDown();
    }
} // namespace Ignosi::Libraries::ECS
