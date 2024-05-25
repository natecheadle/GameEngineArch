#pragma once

#include "FloatComp.hpp"

#include <fmt/format.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>

namespace Ignosi::Libraries::Math
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

        template <typename... Args>
        constexpr Vector(Args... args)
        {
            m_Data = {{args...}};
        }

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

        T dot(const Vector& other) const
        {
            T sum{0};
            for (size_t i = 0; i < other.size(); ++i)
            {
                sum += other.at(i) * at(i);
            }
            return sum;
        }

        T distance(const Vector& other) const
        {
            T sum = 0.0f;
            for (size_t i = 0; i < SIZE; ++i)
            {
                T temp = (other.at(i) - at(i));
                sum += temp * temp;
            }

            return std::sqrt(sum);
        }

        Vector normalize() const
        {
            Vector rslt(*this);
            return rslt / magnitude();
        }

        Vector& normalize_this()
        {
            *this /= magnitude();
            return *this;
        }

        constexpr T x() const
        {
            static_assert(SIZE > 0 && SIZE <= 4, "x() requires a vector of size > 0 and <=4");
            return at(0U);
        }

        constexpr void x(T val)
        {

            static_assert(SIZE > 0 && SIZE <= 4, "x() requires a vector of size > 0 and <=4");
            at(0U) = val;
        }

        constexpr T y() const
        {
            static_assert(SIZE > 1 && SIZE <= 4, "y() requires a vector of size > 1 and <=4");
            return at(1U);
        }

        constexpr void y(T val)
        {
            static_assert(SIZE > 1 && SIZE <= 4, "y() requires a vector of size > 1 and <=4");
            at(1U) = val;
        }

        constexpr T z() const
        {
            static_assert(SIZE > 2 && SIZE <= 4, "z() requires a vector of size > 2 and <=4");
            return at(2U);
        }

        constexpr void z(T val)
        {
            static_assert(SIZE > 2 && SIZE <= 4, "z() requires a vector of size > 2 and <=4");
            at(2U) = val;
        }

        constexpr T w() const
        {
            static_assert(SIZE == 4, "w() requires a vector of size == 4");
            return at(3U);
        }

        constexpr void w(T val)
        {
            static_assert(SIZE == 4, "w() requires a vector of size == 4");
            at(3U) = val;
        }

        Vector perp() const
        {
            static_assert(SIZE == 2, "perp requires a vector of size 2");
            Vector perp(T(1), T(1));
            if (x() != T(0))
            {
                perp[0] = -y() / x();
            }
            else
            {
                perp[1] = -x() / y();
            }

            return perp;
        }

        Vector cross(const Vector& other) const
        {
            static_assert(SIZE == 3, "perp requires a vector of size 3");
            Vector rslt;
            rslt.x((y() * other.z()) - (z() * other.y()));
            rslt.y((z() * other.x()) - (x() * other.z()));
            rslt.z((x() * other.y()) - (y() * other.x()));

            return rslt;
        }

        Vector<3, T> ToVector3() const
        {
            static_assert(SIZE == 4, "ToVector3 requires a vector of size 4");
            return Vector<3, T>(x(), y(), z());
        }

        Vector<3, T> ToVector3(T z) const
        {
            static_assert(SIZE == 2, "ToVector3 requires a vector of size 2");
            return Vector<4, T>(x(), y(), z(), w);
        }

        Vector<4, T> ToVector4(T z, T w) const
        {
            static_assert(SIZE == 2, "ToVector4 requires a vector of size 4");
            return Vector<4, T>(x(), y(), z, w);
        }

        Vector<4, T> ToVector4(T w) const
        {
            static_assert(SIZE == 3, "ToVector3 requires a vector of size 4");
            return Vector<4, T>(x(), y(), z(), w);
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

    template <typename T = float>
    using Vector2 = Vector<2, T>;

    template <typename T = float>
    using Vector3 = Vector<3, T>;

    template <typename T = float>
    using Vector4 = Vector<4, T>;

} // namespace Ignosi::Libraries::Math
