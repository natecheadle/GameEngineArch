#pragma once

#include "EventSubscriber.hpp"
#include "HitShape.h"
#include "KinematicData.h"

#include <Event.hpp>
#include <LinearAlgebra/Vector2.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace Ignosi::Modules::Physics
{
    class PhysicsSystem;

    class RigidBody2D
    {
        std::unique_ptr<Physics::HitShape> m_HitShape;
        bool                               m_IsFixed{true};
        KinematicData*                     m_pPosition;

        Messaging::Event<const RigidBody2D&> m_OnCollisionEvent;

        friend PhysicsSystem;

      public:
        RigidBody2D() = default;
        RigidBody2D(KinematicData* pPosition);

        RigidBody2D(const RigidBody2D& other) = delete;
        RigidBody2D(RigidBody2D&& other)      = default;

        RigidBody2D& operator=(const RigidBody2D& other) = delete;
        RigidBody2D& operator=(RigidBody2D&& other)      = default;

        void Update(float dt);

        std::unique_ptr<Messaging::EventSubscriber<const RigidBody2D&>> SubscribeOnCollision(
            std::function<void(const RigidBody2D& other)> func)
        {
            return m_OnCollisionEvent.Subscribe(std::move(func));
        }

        void IsFixed(bool val) { m_IsFixed = val; }
        bool IsFixed() const { return m_IsFixed; }

        const Vector3<float>& Position() const { return m_pPosition->Position(); }
        const Vector3<float>& Velocity() const { return m_pPosition->Velocity(); }

        void                                      HitShape(std::unique_ptr<Physics::HitShape> pVal) { m_HitShape = std::move(pVal); }
        const std::unique_ptr<Physics::HitShape>& HitShape() const { return m_HitShape; }

      private:
        void CollisionOccurred(const RigidBody2D& other);
    };
} // namespace Ignosi::Modules::Physics
