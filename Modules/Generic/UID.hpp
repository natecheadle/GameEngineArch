#pragma once

#include <atomic>
#include <cstdint>

namespace nate::Modules
{
    template <typename T>
    std::uint64_t UID()
    {
        static std::atomic<uint64_t> nextID = 0;
        return nextID++;
    }
} // namespace nate::Modules