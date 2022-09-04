#include "RendererSM_Processing_SubmittingObjects.h"

#include "Shader/BGFX_Shader.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    RendererSM_Processing_SubmittingObjects::RendererSM_Processing_SubmittingObjects()
    {
        // TODO shaders should be part of the material attached to the object
        Render::BGFX_Shader fragmentShader("fs_cubes.sc.bin", context<RendererSM>().ShaderDir());
        Render::BGFX_Shader vertexShader("vs_cubes.sc.bin", context<RendererSM>().ShaderDir());
        bgfx::ProgramHandle program = bgfx::createProgram(vertexShader.Handle(), fragmentShader.Handle(), true);

        auto queue = context<RendererSM>().GetQueue();
        while (!queue.first.empty())
        {
            const Object3D* pObject = queue.first.front().get();
            queue.first.pop();
            if (!pObject)
                continue;

            bgfx::setTransform(pObject->Transformation().Data().data());

            bgfx::setVertexBuffer(0, pObject->VertexBufferHandle());
            bgfx::setIndexBuffer(pObject->IndexBufferHandle());

            bgfx::submit(0, program);
        }
        context<RendererSM>().PostEvent(RendererSM_EV_ObjectsSubmitted());
    }
} // namespace nate::Modules::Render