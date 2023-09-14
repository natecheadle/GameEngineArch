#pragma once

#include "3D/VertexData.h"

#include <LinearAlgebra/Vector2.hpp>
#include <LinearAlgebra/Vector3.hpp>
#include <Units/Radian.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace Ignosi::Modules::Render
{
    static constexpr std::array<Vector2<float>, 6> RectangleVertexes = {
        {
         {-0.5f, 0.5f},
         {0.5f, -0.5f},
         {-0.5f, -0.5f},
         {-0.5f, 0.5f},
         {0.5f, 0.5f},
         {0.5f, -0.5f},
         }
    };

    static constexpr std::array<Vector3<float>, 36> CubeVertexes = {
        {
         {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
         {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
         {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f},
         {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
         {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f},
         {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
         {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
         {-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
         {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f},
         }
    };

    template <size_t VERT_COUNT>
    static constexpr std::array<Vector2<float>, 3 * (VERT_COUNT - 2)> GenerateCircleVertexes() noexcept
    {
        float angle = 360.0f / (float)VERT_COUNT;

        std::uint32_t triangleCount = (std::uint32_t)VERT_COUNT - 2;

        std::array<Vector2<float>, VERT_COUNT> temp;
        // positions
        for (std::uint32_t i = 0; i < (std::uint32_t)VERT_COUNT; i++)
        {
            float currentAngle = angle * i;
            temp[i].x(cos(Radian<float>(currentAngle)));
            temp[i].y(sin(Radian<float>(currentAngle)));
        }

        std::array<Vector2<float>, 3 * (VERT_COUNT - 2)> vertices;
        size_t                                           index = 0;

        for (int i = 0; i < triangleCount; i++)
        {
            vertices[index++] = temp[0];
            vertices[index++] = temp[i + 1];
            vertices[index++] = temp[i + 2];
        }

        return vertices;
    }
} // namespace Ignosi::Modules::Render
