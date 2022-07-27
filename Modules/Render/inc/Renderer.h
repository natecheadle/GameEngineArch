#include "CursorPosition.hpp"
#include "IRenderer.h"
#include "KeyPressedInfo.hpp"
#include "MouseClickedInfo.hpp"
#include "Object3D.h"
#include "WindowSize.hpp"

#include <IWindow.h>
#include <Job.h>

#include <atomic>
#include <filesystem>
#include <queue>
#include <shared_mutex>
#include <thread>

namespace nate::Modules::Render {
    class Renderer
        : public IRenderer
        , private Jobs::Job {
      public:
        virtual ~Renderer() { PrivShutdown(); }

        void Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc) override;
        bool IsInitialized() const { return m_RendererInitialized; }
        bool IsRunning() const override { return IsExecuting(); }
        void Shutdown() override { PrivShutdown(); }

        void RenderObject(const Object3D* pObject);

        static void RenderFrame();

      protected:
        void ExecuteJob() final;

      private:
        void                        PrivShutdown();
        GUI::IWindow*               m_pWindow{nullptr};
        std::filesystem::path       m_ShaderLoc;
        std::shared_mutex           m_CallbackMutex;
        GUI::MouseClickedInfo       m_LastMouseClick;
        GUI::CursorPosition         m_LastPosition;
        GUI::WindowSize             m_WindowSize;
        float                       m_CamYaw{0.0};
        float                       m_CamPitch{0.0};
        std::atomic<bool>           m_WindowShouldClose{false};
        std::atomic<bool>           m_RendererInitialized{false};
        std::mutex                  m_RenderObjectsMutex;
        std::queue<const Object3D*> m_ObjectsToRender;
    };
} // namespace nate::Modules::Render
