#include "HitCircle.h"

#include <3D/GenericVertexes.hpp>
namespace Ignosi::Modules::Physics
{
    namespace
    {
        static constexpr auto CIRCLE_VERTS = Render::GenerateCircleVertexes<16>();
    }

    std::unique_ptr<DebugSpriteEntity> HitCircle::CreateDebugSprite() const
    {
        return nullptr;
    }
} // namespace Ignosi::Modules::Physics
