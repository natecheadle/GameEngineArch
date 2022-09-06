#include "RendererSM_Initialized.h"

#include "StateMachine/RendererSM.h"

#include <mutex>
#include <thread>

namespace nate::Modules::Render
{
    RendererSM_Initialized::RendererSM_Initialized(my_context ctx)
        : my_base(ctx)
    {
        context<RendererSM>().InitializationComplete();
        while (!context<RendererSM>().ShouldStart())
        {
            BuildShaders();
            BuildPrograms();
            std::this_thread::yield();
        }
        context<RendererSM>().IncrementFrame();
        context<RendererSM>().PostEvent(RendererSM_EV_StartRunning());
    }

    void RendererSM_Initialized::BuildShaders()
    {
        RendererSM::ShaderContext* pContext{nullptr};
        do
        {
            pContext = context<RendererSM>().PopShaderContext();
            if (pContext)
            {
                {
                    std::unique_lock<std::mutex> lock(pContext->DataMutex);
                    pContext->Handle = RendererSM::GetShaderHandle(pContext->Data);
                }
                pContext->DataCondition.notify_all();
            }
        } while (pContext != nullptr);
    }

    void RendererSM_Initialized::BuildPrograms()
    {
        RendererSM::ProgramContext* pContext{nullptr};
        do
        {
            pContext = context<RendererSM>().PopProgramContext();
            if (pContext)
            {
                {
                    std::unique_lock<std::mutex> lock(pContext->DataMutex);
                    pContext->Handle = RendererSM::GetProgramHandle(pContext->FragmentShader, pContext->VertexShader);
                }
                pContext->DataCondition.notify_all();
            }
        } while (pContext != nullptr);
    }
} // namespace nate::Modules::Render