#include "Camera3D.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace nate::Modules::Render {
    Camera3D::Camera3D()
    {
        bx::mtxProj(
            m_Projection.Data().data(),
            60.0f,
            float(16) / float(9),
            0.1f,
            100.0f,
            bgfx::getCaps()->homogeneousDepth);
    }

} // namespace nate::Modules::Render