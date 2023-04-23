#include "ILogManager.h"

#include "LogManager.h"

namespace nate::Modules::Debug::Logging
{
    std::unique_ptr<ILogManager> ILogManager::Factory()
    {
        return std::make_unique<LogManager>();
    }

} // namespace nate::Modules::Debug::Logging