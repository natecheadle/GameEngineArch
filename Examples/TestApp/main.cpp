#include <IWindow.h>

#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
    std::unique_ptr<Ignosi::Libraries::Renderer::IWindow> pWindow = Ignosi::Libraries::Renderer::IWindow::Create("test_window");

    unsigned int update = 0;
    while (!pWindow->ShouldClose())
    {
        if (update % 60 == 0)
        {
            std::cout << "Window.Name: " << pWindow->Name() << " Window.Size: " << pWindow->ActualWindowSize() << std::endl;
        }
        pWindow->SwapBuffers();
        pWindow->PollEvents();
        update++;
    }
}
