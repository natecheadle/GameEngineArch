#include "KinematicData.h"

namespace Ignosi::Modules::Physics
{
    void KinematicData::Update(double dt)
    {
        Second<float> dtf(static_cast<float>(dt));

        m_Data.Position = m_Data.Position + (m_Data.Velocity * dtf.Seconds());
        m_Data.Velocity = m_Data.Velocity + (m_Data.Acceleration * dtf.Seconds());

        m_Data.Angle           = m_Data.Angle + (m_Data.AngularVelocity * dtf);
        m_Data.AngularVelocity = m_Data.AngularVelocity + (m_Data.AngularAcceleration * dtf);
    }
} // namespace Ignosi::Modules::Physics
