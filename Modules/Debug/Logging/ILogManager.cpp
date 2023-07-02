#include "ILogManager.h"

#include "LogManager.h"

namespace Ignosi::Modules::Debug::Logging
{
    std::unique_ptr<ILogManager> ILogManager::Factory()
    {
        return std::make_unique<LogManager>();
    }

} // namespace Ignosi::Modules::Debug::Logging