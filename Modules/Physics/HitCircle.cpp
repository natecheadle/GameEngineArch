#include "HitCircle.h"

#include <3D/GenericVertexes.hpp>
namespace Ignosi::Modules::Physics
{
    std::unique_ptr<DebugSpriteEntity> HitCircle::CreateDebugSprite() const
    {
        auto circle_verts = Render::GenerateCircleVertexes<16>();

        return nullptr;
    }
} // namespace Ignosi::Modules::Physics
