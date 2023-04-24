#pragma once

#include <UID.hpp>

namespace nate::Modules::ECS
{
    class System
    {
        std::uint64_t m_ID{UID<System>()};

      public:
        System()          = default;
        virtual ~System() = default;

        System(const System& other) = delete;
        System(System&& other)      = default;

        System& operator=(const System& other) = delete;
        System& operator=(System&& other)      = default;

        std::uint64_t ID() const { return m_ID; }
    };
} // namespace nate::Modules::ECS
