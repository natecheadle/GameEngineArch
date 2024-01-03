#pragma once

namespace Ignosi::Libraries
{
    template <class T, class PARENT, class RETURN = const T&>
    class Property_RW
    {
        friend PARENT;

        T m_Field;

      public:
        virtual ~Property_RW() = default;

        bool         operator==(const Property_RW& rhs) const { return *this == rhs.m_Field; }
        virtual bool operator==(const T& rhs) const { return m_Field == rhs; }

        virtual T& operator=(const T& value) { return m_Field = value; }

        virtual RETURN operator()() const { return m_Field; }
        explicit       operator RETURN() const { return m_Field; }
    };
} // namespace Ignosi::Libraries
