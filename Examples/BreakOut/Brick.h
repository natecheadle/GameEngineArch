#pragma once

#include "BreakOutEntity.h"

namespace nate::BreakOut
{
    enum class BrickType
    {
        None     = 0,
        Solid    = 1,
        BabyBlue = 2,
        Green    = 3,
    };

    class Brick : public BreakOutEntity
    {
        BrickType m_Type;

      public:
        Brick(Modules::Memory::pool_pointer<Modules::Render::Sprite>&& val)
            : BreakOutEntity(std::move(val))
        {
        }

        BrickType Type() const { return m_Type; }
        void      Type(BrickType val) { m_Type = val; }

        Modules::Render::Sprite&       Sprite() { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
    };
} // namespace nate::BreakOut
