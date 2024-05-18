#pragma once

namespace Ignosi::Libraries::ECS
{
    class App
    {
      public:
        App()  = default;
        ~App() = default;

        App(const App& other) = delete;
        App(App&& other)      = default;

        App& operator=(const App& other) = delete;
        App& operator=(App&& other)      = default;

        void Run();

      protected:
        virtual bool KeepUpdating() = 0;

        virtual void Initialize() = 0;
        virtual void Update()     = 0;
        virtual void ShutDown()   = 0;
    };
} // namespace Ignosi::Libraries::ECS