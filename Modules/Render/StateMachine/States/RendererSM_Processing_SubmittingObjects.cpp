#include "RendererSM_Processing_SubmittingObjects.h"

#include "Shader/BGFX_Shader.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    RendererSM_Processing_SubmittingObjects::RendererSM_Processing_SubmittingObjects(my_context ctx)
        : my_base(std::move(ctx))
    {
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

            bgfx::submit(0, pObject->GetMaterial()->GetProgram());
        }
        context<RendererSM>().PostEvent(RendererSM_EV_ObjectsSubmitted());
    }
} // namespace nate::Modules::Render