#include "HitCircle.h"

#include "HitShape.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector2.hpp>

#include <vector>

namespace Ignosi::Modules::Physics
{
    HitCircle::HitCircle(const KinematicData* pPosition)
        : HitShape(pPosition)
    {
    }

    std::array<Vector2<float>, 2> HitCircle::ProjectShape(const Vector2<float>& axis)
    {
        Vector3<float>                origin = Origin();
        std::array<Vector2<float>, 2> rslt;
        rslt[0] = Vector2<float>(origin.x(), origin.y());
        rslt[1] = Vector2<float>(origin.x(), origin.y());

        if (axis[0] != 0.0 && axis[1] != 0.0)
        {
            Degree<float> m(axis[1] / axis[0]);
            float         x = origin[0] + cos(m) * m_Radius;
            float         y = origin[1] + sin(m) + m_Radius;

            rslt[0][0] += x;
            rslt[0][1] += y;

            rslt[1][0] -= x;
            rslt[1][1] -= y;
        }
        else if (axis[0] == 0.0)
        {
            rslt[1][0] = 0.0;
            rslt[0][0] = 0.0;

            rslt[1][1] += m_Radius;
            rslt[0][1] -= m_Radius;
        }
        else
        {
            rslt[1][0] += m_Radius;
            rslt[0][0] -= m_Radius;

            rslt[1][1] = 0.0;
            rslt[0][1] = 0.0;
        }

        return rslt;
    }
} // namespace Ignosi::Modules::Physics
