#include "System.h"

#include "IWorld.h"

namespace Ignosi::Modules::ECS
{
    void System::Initialize(std::uint32_t id, IWorld* pWorld)
    {
        m_ID     = id;
        m_pWorld = pWorld;
    }
} // namespace Ignosi::Modules::ECS
