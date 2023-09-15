#include "BreakOutApp.h"
#include "BreakOutEntity.h"

using namespace Ignosi::Modules;

int main()
{
    try
    {
        Ignosi::BreakOut::BreakOutApp app(std::make_unique<Ignosi::BreakOut::BreakOutWorld>(), {800, 600}, "Test Window");
        int                           code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
