
#include "IWindow.h"

#include <filesystem>

namespace nate::Modules::Render {
    class IRenderer {
      public:
        virtual ~IRenderer() = default;

        virtual void Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc) = 0;
        virtual bool IsRunning() const                                                  = 0;
        virtual void Shutdown()                                                         = 0;
    };
} // namespace nate::Modules::Render