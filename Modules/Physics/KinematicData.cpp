#include "KinematicData.h"

#include "LinearAlgebra/Vector3.hpp"

namespace Ignosi::Modules::Physics
{
    void KinematicData::Update(double dt)
    {
        Second<float> dtf(static_cast<float>(dt));

        m_Data.Position = m_Data.Position + (m_Data.Velocity * dtf.Value());
        m_Data.Velocity = m_Data.Velocity + (m_Data.Acceleration * dtf.Value());

        for (size_t i = 0; i < Vector3<Radian<float>>::size(); ++i)
        {
            m_Data.Angle[i] = m_Data.Angle[i] + (m_Data.AngularVelocity[i] * dtf);
            m_Data.Angle[i].Modulo();
            m_Data.AngularVelocity[i] = m_Data.AngularVelocity[i] + (m_Data.AngularAcceleration[i] * dtf);
        }
    }
} // namespace Ignosi::Modules::Physics
