#pragma once

#include <cstdlib>
#include <limits>

namespace Ignosi::Modules::GUI {
    class CursorPosition {
        double m_XPos{0};
        double m_YPos{0};

      public:
        CursorPosition() = default;
        CursorPosition(double xPos, double yPos)
            : m_XPos(xPos)
            , m_YPos(yPos)
        {
        }
        ~CursorPosition() = default;

        CursorPosition(const CursorPosition& other) = default;
        CursorPosition& operator=(const CursorPosition& other) = default;

        bool operator==(const CursorPosition& other) const
        {
            return std::abs(m_XPos - other.m_XPos) < std::numeric_limits<double>::epsilon() &&
                   std::abs(m_YPos - other.m_YPos) < std::numeric_limits<double>::epsilon();
        }

        double XPos() const { return m_XPos; }
        double YPos() const { return m_YPos; }
    };
} // namespace Ignosi::Modules::GUI