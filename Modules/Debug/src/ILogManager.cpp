#include "ILogManager.h"

#include "LogManager.h"

namespace amc::Shared {
    std::unique_ptr<ILogManager> ILogManager::Factory()
    {
        return std::make_unique<LogManager>();
    }

} // namespace amc::Shared