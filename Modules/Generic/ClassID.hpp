#pragma once

#include <cstdint>
#include <limits>

namespace Ignosi::Modules
{
    template <class Derived>
    class ClassID
    {
        static constexpr size_t RESET_VAL = std::numeric_limits<size_t>::max();

      protected:
        ClassID() { ID = RESET_VAL; }
        ClassID(size_t id) { ID = id; }

      public:
        size_t ID;

        void Reset() { ID = RESET_VAL; }
        bool IsValid() const { return ID != RESET_VAL; }

        friend bool        operator==(const ClassID& lhs, const ClassID& rhs) = default;
        friend inline bool operator!=(const ClassID& lhs, const ClassID& rhs) { return !(lhs == rhs); }
        friend inline bool operator<(const ClassID& lhs, const ClassID& rhs) { return lhs.ID < rhs.ID; }
        friend inline bool operator<=(const ClassID& lhs, const ClassID& rhs) { return lhs < rhs || lhs == rhs; }
        friend inline bool operator>(const ClassID& lhs, const ClassID& rhs) { return lhs.ID > rhs.ID; }
        friend inline bool operator>=(const ClassID& lhs, const ClassID& rhs) { return lhs > rhs || lhs == rhs; }
    };
} // namespace Ignosi::Modules