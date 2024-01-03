#pragma once

#include <type_traits>
namespace Ignosi::Libraries
{
    template <class T, class PARENT, class RETURN = const T&>
    class Property_RO
    {
        friend PARENT;
        T m_Field;

      public:
        virtual ~Property_RO() = default;

        bool operator==(const Property_RO& rhs) const { return *this == rhs.m_Field; }

        virtual bool operator==(const T& rhs) const { return m_Field == rhs; }

        virtual RETURN operator()() const { return m_Field; }
        explicit       operator RETURN() const { return m_Field; }
    };
} // namespace Ignosi::Libraries
