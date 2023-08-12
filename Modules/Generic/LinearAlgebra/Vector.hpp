#pragma once

#include "FloatComp.hpp"

#include <fmt/format.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace Ignosi::Modules
{
    template <size_t SIZE, typename T>
    class Vector
    {
        std::array<T, SIZE> m_Data;

      public:
        using const_iterator = typename std::array<T, SIZE>::const_iterator;
        using iterator       = typename std::array<T, SIZE>::iterator;

        constexpr Vector() noexcept { m_Data.fill(T()); }

        constexpr Vector(std::initializer_list<T> vals) noexcept
        {
            assert(vals.size() <= SIZE);
            size_t i = 0;
            for (const auto& val : vals)
            {
                m_Data[i++] = val;
            }
        }

        constexpr Vector(T val) noexcept { m_Data.fill(val); }

        Vector(const Vector& other) noexcept { std::copy(other.begin(), other.end(), m_Data.begin()); }
        Vector(Vector&& other) noexcept = default;

        Vector& operator=(const Vector& other) noexcept = default;
        Vector& operator=(Vector&& other) noexcept      = default;

        static constexpr size_t size() noexcept { return SIZE; }
        T*                      data() noexcept { return m_Data.data(); }
        const T*                data() const noexcept { return m_Data.data(); }

        T magnitude() const
        {
            T sum{0};
            for (size_t i = 0; i < size(); ++i)
            {
                sum += m_Data[i] * m_Data[i];
            }
            return std::sqrt(sum);
        }

        T dot(const Vector& other)
        {
            T sum{0};
            for (size_t i = 0; i < other.size(); ++i)
            {
                sum += other.at(i) * at(i);
            }
            return sum;
        }

        constexpr T&       operator[](size_t i) { return m_Data[i]; }
        constexpr const T& operator[](size_t i) const { return m_Data[i]; }

        constexpr T&       at(size_t i) { return m_Data.at(i); }
        constexpr const T& at(size_t i) const { return m_Data.at(i); }

        iterator       begin() { return m_Data.begin(); }
        const_iterator begin() const { return m_Data.begin(); }
        const_iterator cbegin() const { return m_Data.begin(); }

        iterator       end() { return m_Data.end(); }
        const_iterator end() const { return m_Data.end(); }
        const_iterator cend() const { return m_Data.cend(); }

        friend bool operator==(const Vector& lhs, const Vector& rhs)
        {
            bool rslt{true};

            for (size_t i = 0; i < size(); ++i)
            {
                rslt = rslt && almost_eq<T>(lhs[i], rhs[i]);
            }
            return rslt;
        }

        friend Vector operator-(const Vector& lhs, const Vector& rhs)
        {
            Vector rslt(lhs);
            return rslt -= rhs;
        }

        friend Vector operator+(const Vector& lhs, const Vector& rhs)
        {
            Vector rslt(lhs);
            return rslt += rhs;
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

        friend Vector operator*(const Vector& lhs, T rhs)
        {
            Vector rslt(lhs);
            return rslt *= rhs;
        }

        friend Vector operator*(T lhs, const Vector& rhs)
        {
            Vector rslt(rhs);
            return rslt *= lhs;
        }

        friend Vector operator*=(Vector& lhs, T rhs)
        {
            for (size_t i = 0; i < size(); ++i)
            {
                lhs[i] *= rhs;
            }
            return lhs;
        }

        friend Vector operator/(const Vector& lhs, T rhs)
        {
            Vector rslt(lhs);
            return rslt /= rhs;
        }

        friend Vector operator/=(Vector& lhs, T rhs)
        {
            for (size_t i = 0; i < size(); ++i)
            {
                lhs[i] /= rhs;
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
} // namespace Ignosi::Modules