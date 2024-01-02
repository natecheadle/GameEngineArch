#pragma once

namespace Ignosi::Libraries
{
    template <class T>
    class Property_RW
    {
        T m_Field;

      public:
        Property_RW(const T& value)
            : m_Field(value)
        {
        }

        Property_RW() = default;

        ~Property_RW() = default;

        Property_RW(const Property_RW& other) = default;
        Property_RW(Property_RW&& other)      = default;

        Property_RW& operator=(const Property_RW& other) = default;
        Property_RW& operator=(Property_RW&& other)      = default;

        friend bool operator==(const Property_RW& lhs, const Property_RW& rhs) = default;

        const T& operator()() { return GetValue(); }
        void     operator()(const T& value) { SetValue(value); }

      protected:
        virtual const T& GetValue() const { return m_Field; }
        virtual void     SetValue(const T& value) { m_Field = value; }
    };
} // namespace Ignosi::Libraries
