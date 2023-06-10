#pragma once

#include "BreakOutEntity.h"

namespace nate::BreakOut
{
    class Background : Modules::ECS::Entity<Modules::Render::Sprite>
    {
      public:
        Background(Modules::Memory::pool_pointer<Modules::Render::Sprite>&& val)
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
