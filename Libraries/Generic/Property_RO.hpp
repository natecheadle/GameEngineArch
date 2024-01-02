#pragma once

namespace Ignosi::Libraries
{
    template <class T>
    class Property_RO
    {
        T m_Field;

      public:
        Property_RO(const T& value)
            : m_Field(value)
        {
        }

        Property_RO() = default;

        ~Property_RO() = default;

        Property_RO(const Property_RO& other) = default;
        Property_RO(Property_RO&& other)      = default;

        Property_RO& operator=(const Property_RO& other) = default;
        Property_RO& operator=(Property_RO&& other)      = default;

        friend bool operator==(const Property_RO& lhs, const Property_RO& rhs) = default;

        const T& operator()() { return GetValue(); }

      protected:
        virtual const T& GetValue() const { return m_Field; }
    };
} // namespace Ignosi::Libraries
