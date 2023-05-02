#pragma once

#include "FloatComp.hpp"

#include <fmt/format.h>

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <type_traits>

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

        static constexpr size_t size() { return SIZE; }
        T*                      data() { return m_Data.data(); }
        const T*                data() const { return m_Data.data(); }

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
} // namespace nate::Modules