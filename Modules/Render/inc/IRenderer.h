
#include "IWindow.h"
namespace nate::Modules::Render {
    class IRenderer {
      public:
        virtual ~IRenderer() = default;

        virtual void Initialize(GUI::IWindow* pWindow) = 0;
        virtual bool IsRunning() const                 = 0;
        virtual void Shutdown()                        = 0;
    };
} // namespace nate::Modules::Render