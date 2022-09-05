#pragma once

#include "3D/Camera/Camera3D.h"
#include "3D/Object3D.h"
#include "IWindow.h"

#include <filesystem>
#include <memory>

namespace nate::Modules::Render
{
    class IRenderer
    {
      public:
        virtual ~IRenderer() = default;

        virtual void Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc) = 0;
        virtual bool IsRunning() const                                                  = 0;
        virtual void StartRendering()                                                   = 0;
        virtual void Shutdown()                                                         = 0;

        virtual bool                  RenderingFailed() const = 0;
        virtual const std::exception& GetFailure() const      = 0;

        virtual void AttachCamera(std::shared_ptr<const Camera3D> pCamera) = 0;
        virtual void RenderObject(std::shared_ptr<const Object3D> pObject) = 0;

        virtual void RenderFrame() = 0;
    };
} // namespace nate::Modules::Render