#pragma once

#include <fmt/format.h>

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iomanip>
#include <iostream>

namespace nate::Modules
{
    template <size_t SIZE, typename T = float>
    class Vector
    {
        std::array<T, SIZE> m_Data;

      public:
        using const_iterator = typename std::array<T, SIZE>::const_iterator;
        using iterator       = typename std::array<T, SIZE>::iterator;

        Vector() { m_Data.fill(T()); }

        Vector(std::array<T, SIZE> init)
            : m_Data(init)
        {
        }

        template <typename IT>
        Vector(IT begin, IT end)
            : m_Data(begin, end)
        {
        }

        Vector(const Vector& other) = default;
        Vector(Vector&& other)      = default;

        Vector& operator=(const Vector& other) = default;
        Vector& operator=(Vector&& other)      = default;

        float*       data() { return m_Data.data(); }
        const float* data() const { return m_Data.data(); }

        T dot(const Vector& other)
        {
            T sum{0};
            for (size_t i = 0; i < other.size(); ++i)
            {
                sum += other.at(i) * at(i);
            }
        }

        T&       operator[](size_t i) { return m_Data[i]; }
        const T& operator[](size_t i) const { return m_Data[i]; }

        T&       at(size_t i) { return m_Data.at(i); }
        const T& at(size_t i) const { return m_Data.at(i); }

        iterator       begin() { return m_Data.begin(); }
        const_iterator begin() const { return m_Data.begin(); }
        const_iterator cbegin() const { return m_Data.begin(); }

        iterator       end() { return m_Data.end(); }
        const_iterator end() const { return m_Data.end(); }
        const_iterator cend() const { return m_Data.cend(); }

        friend bool operator==(const Vector& lhs, const Vector& rhs) = default;

        friend Vector operator-(const Vector& lhs, const Vector& rhs)
        {
            Vector rslt(lhs);
            for (size_t i = 0; i < lhs.size(); ++i)
            {
                rslt[i] -= rhs[i];
            }
            return rslt;
        }

        friend Vector operator+(const Vector& lhs, const Vector& rhs)
        {
            Vector rslt(lhs);
            for (size_t i = 0; i < lhs.size(); ++i)
            {
                rslt[i] += rhs[i];
            }
            return rslt;
        }

        friend Vector& operator-=(Vector& lhs, const Vector& rhs)
        {
            for (size_t i = 0; i < lhs.size(); ++i)
            {
                lhs[i] -= rhs[i];
            }
            return lhs;
        }

        friend Vector& operator+=(Vector& lhs, const Vector& rhs)
        {
            for (size_t i = 0; i < lhs.size(); ++i)
            {
                lhs[i] += rhs[i];
            }
            return lhs;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector& vec)
        {
            for (const auto& val : vec)
            {
                static constexpr int ColLength{10};
                os << fmt::format("| {:^{}.2f} |\n", val, ColLength);
            }
            return os;
        }
    };
} // namespace nate::Modules