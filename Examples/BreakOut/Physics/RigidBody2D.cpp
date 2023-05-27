#include "RigidBody2D.h"

namespace nate::Modules::Physics
{
    void RigidBody2D::CollisionOccurred(const RigidBody2D& other)
    {
        if (!IsFixed())
        {
            std::array<Vector2<float>, 4> compass = {
                Vector2<float>{0.0f,  1.0f }, // up
                Vector2<float>{1.0f,  0.0f }, // right
                Vector2<float>{0.0f,  -1.0f}, // down
                Vector2<float>{-1.0f, 0.0f }  // left
            };
            float        max        = 0.0f;
            unsigned int best_match = -1;
            auto         target     = other.Position();
            for (unsigned int i = 0; i < 4; i++)
            {
                float dot_product = glm::dot(glm::normalize(target), compass[i]);
                if (dot_product > max)
                {
                    max        = dot_product;
                    best_match = i;
                }
            }

            if (other.IsFixed())
            {
            }
            else
            {
            }
        }

        m_OnCollision(other);
    }
} // namespace nate::Modules::Physics
