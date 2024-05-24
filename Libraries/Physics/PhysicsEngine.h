#pragma once

#include "IPhysicsEngine.h"

namespace Ignosi::Libraries::Physics
{
    class PhysicsEngine : public IPhysicsEngine
    {
      public:
        ~PhysicsEngine() override = default;

        void Update(std::chrono::milliseconds delta) override;
    };
} // namespace Ignosi::Libraries::Physics
