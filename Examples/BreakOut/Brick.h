#pragma once

#include <3D/Sprite.h>
#include <Entity.h>

namespace nate::BreakOut
{
    class Brick : Modules::ECS::Entity<Modules::Render::Sprite>
    {
      public:
        Brick(Modules::Memory::pool_pointer<Modules::Render::Sprite>&& val)
            : Modules::ECS::Entity<Modules::Render::Sprite>(std::move(val))
        {
        }

        Modules::Render::Sprite& Sprite()
        {
            return Modules::ECS::Entity<Modules::Render::Sprite>::Get<Modules::Render::Sprite>();
        }
        const Modules::Render::Sprite& Sprite() const
        {
            return Modules::ECS::Entity<Modules::Render::Sprite>::Get<Modules::Render::Sprite>();
        }
    };
} // namespace nate::BreakOut
