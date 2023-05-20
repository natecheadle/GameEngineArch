#pragma once

#include "BreakOutEntity.h"

namespace nate::BreakOut
{
    class Brick : BreakOutEntity
    {
      public:
        Brick(Modules::Memory::pool_pointer<Modules::Render::Sprite>&& val)
            : BreakOutEntity(std::move(val))
        {
        }

        Modules::Render::Sprite&       Sprite() { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
    };
} // namespace nate::BreakOut
