#include "HitShape.h"

namespace Ignosi::Modules::Physics
{
    void HitShape::EnableDebugMode()
    {
        m_IsDebugModeEnabled = true;
    }

    void HitShape::DisableDebugMode()
    {
        m_IsDebugModeEnabled = false;
    }
} // namespace Ignosi::Modules::Physics
