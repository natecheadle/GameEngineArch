#pragma once

#include <atomic>
#include <cstdint>

namespace Ignosi::Modules
{
    inline std::uint64_t UID()
    {
        static std::atomic<uint64_t> nextID = 0;
        return nextID++;
    }
} // namespace Ignosi::Modules