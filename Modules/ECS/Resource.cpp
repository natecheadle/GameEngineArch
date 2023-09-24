#include "Resource.h"

namespace Ignosi::Modules::ECS
{

    Resource::Resource(const std::string& name)
    {
        m_ID.Name = name;
    }
} // namespace Ignosi::Modules::ECS
