#pragma once

#include "KinematicData.h"
#include "LinearAlgebra/Vector3.hpp"

#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector2.hpp>
#include <Units/Radian.hpp>

#include <array>
#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitShape
    {
        const KinematicData* m_pPosition;
        Radian<float>        m_Rotation;

      public:
        virtual ~HitShape() = default;

        const Vector3<Radian<float>>& Rotation() const { return m_pPosition->Angle(); }
        const Vector3<float>&         Origin() const { return m_pPosition->Position(); }

        virtual float                              Radius() const = 0;
        virtual const std::vector<Vector2<float>>& Corners()      = 0;
        virtual const std::vector<Vector2<float>>& TestAxes()     = 0;

        virtual std::array<Vector2<float>, 2> ProjectShape(const Vector2<float>& axis);

      protected:
        HitShape(const KinematicData* pPosition);
    };
} // namespace Ignosi::Modules::Physics
