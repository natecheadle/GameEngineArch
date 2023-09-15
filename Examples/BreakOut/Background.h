#pragma once

#include "BreakOutEntity.h"
#include "KinematicData.h"

#include <3D/Sprite.h>
#include <World.h>

namespace Ignosi::BreakOut
{
    class Background : public CustomBreakOutEntity
    {
      public:
        Background(BreakOutEntityPointer&& val, Modules::Render::Renderer* pRenderer, float aspectRatio)
            : CustomBreakOutEntity(std::move(val))
        {
            World()->AddComponent<Modules::Physics::KinematicData>(Entity());
            World()->AddComponent<Modules::Render::Sprite>(
                Entity(),
                Modules::Render::Sprite(pRenderer, GetComponent<Modules::Physics::KinematicData>(), aspectRatio));
        }

        Modules::ECS::WeakComponentPointer<Modules::Render::Sprite> Sprite() const { return GetComponent<Modules::Render::Sprite>(); }
        Modules::ECS::WeakComponentPointer<Modules::Physics::KinematicData> KinematicData() const
        {
            return GetComponent<Modules::Physics::KinematicData>();
        }

      protected:
        void OnUpdate(double dt) override {}
    };
} // namespace Ignosi::BreakOut
