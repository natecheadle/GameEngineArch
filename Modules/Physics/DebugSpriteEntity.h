#pragma once

#include <3D/Sprite.h>
#include <Entity.h>

namespace Ignosi::Modules::Physics
{

    class DebugSpriteEntity : ECS::Entity<Render::Sprite>
    {
      public:
        DebugSpriteEntity(Memory::pool_pointer<Render::Sprite>&& val)
            : ECS::Entity<Render::Sprite>(std::move(val))
        {
        }

        Render::Sprite&       Sprite() { return ECS::Entity<Render::Sprite>::Get<Render::Sprite>(); }
        const Render::Sprite& Sprite() const { return ECS::Entity<Render::Sprite>::Get<Render::Sprite>(); }
    };
} // namespace Ignosi::Modules::Physics
