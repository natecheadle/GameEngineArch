#pragma once

#include "Vector.hpp"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

namespace Ignosi::Modules
{
    template <size_t SIZE, typename T = float>
    class SquareMatrix
    {
        std::array<Vector<SIZE, T>, SIZE> m_Data;

      public:
        SquareMatrix() = default;

        SquareMatrix(std::array<Vector<SIZE, T>, SIZE> init) noexcept
            : m_Data(init)
        {
        }

        template <typename IT>
        SquareMatrix(IT begin, IT end)
            : m_Data(begin, end)
        {
        }

        SquareMatrix(const SquareMatrix& other)     = default;
        SquareMatrix(SquareMatrix&& other) noexcept = default;

        template <class DERIVED>
        static DERIVED identity()
        {
            static_assert(std::is_base_of_v<SquareMatrix, DERIVED>, "return type must derive from SquareMatrix");
            DERIVED rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                rslt[i][i] = 1;
            }
            return rslt;
        }

        SquareMatrix& operator=(const SquareMatrix& other)     = default;
        SquareMatrix& operator=(SquareMatrix&& other) noexcept = default;

        static constexpr size_t size() { return SIZE; }

        float*       raw_data() { return reinterpret_cast<float*>(m_Data.data()); }
        const float* raw_data() const { return reinterpret_cast<const float*>(m_Data.data()); }

        Vector<SIZE, T>*       data() { return m_Data.data(); }
        const Vector<SIZE, T>* data() const { return m_Data.data(); }

        Vector<SIZE, T>&       operator[](size_t i) { return m_Data[i]; }
        const Vector<SIZE, T>& operator[](size_t i) const { return m_Data[i]; }

        Vector<SIZE, T>&       at(size_t i) { return m_Data.at(i); }
        const Vector<SIZE, T>& at(size_t i) const { return m_Data.at(i); }

        T determinant() const
        {
            if constexpr (SIZE == 1)
            {
                return m_Data[0][0];
            }
            if constexpr (SIZE == 2)
            {
                return m_Data[0][0] * m_Data[1][1] - m_Data[0][1] * m_Data[1][0];
            }

            std::array<Vector<SIZE, T>, SIZE * 2> tmp;
            for (size_t i = 0; i < m_Data.size(); ++i)
            {
                for (size_t j = 0; j < m_Data.size(); ++j)
                {
                    tmp[i][j]        = m_Data[i][j];
                    tmp[i + SIZE][j] = m_Data[i][j];
                }
            }

            std::array<T, SIZE * 2> intermed;
            for (size_t i = 0; i < tmp.size(); ++i)
            {
                intermed[i] = i < SIZE ? 1 : -1;
            }

            for (size_t i = 0; i < m_Data.size(); ++i)
            {
                for (size_t j = 0; j < m_Data.size(); ++j)
                {
                    intermed[i] *= tmp[i + j][j];
                    intermed[(SIZE * 2 - 1) - i] *= tmp[(SIZE * 2 - 1) - (i + j)][j];
                }
            }

            return std::accumulate(intermed.begin(), intermed.end(), 0.0);
        }

        SquareMatrix invert() const
        {
            T det = determinant();
            if (det == 0)
                throw std::invalid_argument("Matrix determinant must not be zero to calculate inverse.");

            SquareMatrix rslt = adjunct();
            return rslt / det;
        }

        SquareMatrix& invert_this()
        {
            auto rslt    = invert();
            this->m_Data = rslt.m_Data;
            return *this;
        }

        SquareMatrix transpose() const
        {
            SquareMatrix rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    rslt[j][i] = m_Data[i][j];
                }
            }
            return rslt;
        }

        SquareMatrix& transpose_this()
        {
            std::array<Vector<SIZE, T>, SIZE> tmp = m_Data;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    m_Data[j][i] = tmp[i][j];
                }
            }
            return *this;
        }

        std::string print() const
        {
            std::stringstream ss;
            ss << *this;

            return ss.str();
        }

        friend bool operator==(const SquareMatrix& lhs, const SquareMatrix& rhs) = default;

        friend SquareMatrix operator*(const SquareMatrix& lhs, const SquareMatrix& rhs)
        {
            SquareMatrix rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    for (size_t k = 0; k < SIZE; ++k)
                    {
                        rslt[j][i] += lhs[k][i] * rhs[j][k];
                    }
                }
            }
            return rslt;
        }

        friend SquareMatrix operator-(const SquareMatrix& lhs, const SquareMatrix& rhs)
        {
            SquareMatrix rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    rslt[i][j] = lhs[i][j] - rhs[i][j];
                }
            }
            return rslt;
        }

        friend SquareMatrix operator+(const SquareMatrix& lhs, const SquareMatrix& rhs)
        {
            SquareMatrix rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    rslt[i][j] = lhs[i][j] + rhs[i][j];
                }
            }
            return rslt;
        }

        friend SquareMatrix& operator*=(SquareMatrix& lhs, const SquareMatrix& rhs)
        {
            lhs = lhs * rhs;
            return lhs;
        }

        friend SquareMatrix& operator-=(SquareMatrix& lhs, const SquareMatrix& rhs)
        {
            lhs = lhs + rhs;
            return lhs;
        }

        friend SquareMatrix& operator+=(SquareMatrix& lhs, const SquareMatrix& rhs)
        {
            lhs = lhs - rhs;
            return lhs;
        }

        friend SquareMatrix operator*(const SquareMatrix& lhs, T rhs)
        {
            SquareMatrix rslt(lhs);
            rslt += rhs;
            return rslt;
        }

        friend SquareMatrix operator*(T lhs, const SquareMatrix& rhs)
        {
            SquareMatrix rslt(rhs);
            rslt += lhs;
            return rslt;
        }

        friend SquareMatrix& operator*=(SquareMatrix& lhs, T rhs)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    lhs[i][j] = lhs[i][j] * rhs;
                }
            }
            return lhs;
        }

        friend SquareMatrix operator/(const SquareMatrix& lhs, T rhs)
        {
            SquareMatrix rslt(lhs);
            rslt /= rhs;
            return rslt;
        }

        friend SquareMatrix& operator/=(SquareMatrix& lhs, T rhs)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    lhs[i][j] = lhs[i][j] / rhs;
                }
            }
            return lhs;
        }

        friend SquareMatrix operator+(const SquareMatrix& lhs, T rhs)
        {
            SquareMatrix rslt(lhs);
            rslt += rhs;
            return rslt;
        }

        friend SquareMatrix operator+(T lhs, const SquareMatrix& rhs)
        {
            SquareMatrix rslt(rhs);
            rslt += lhs;
            return rslt;
        }

        friend SquareMatrix& operator+=(SquareMatrix& lhs, T rhs)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    lhs[i][j] = lhs[i][j] + rhs;
                }
            }
            return lhs;
        }

        friend SquareMatrix operator-(const SquareMatrix& lhs, T rhs)
        {
            SquareMatrix rslt(lhs);
            rslt -= rhs;
            return rslt;
        }

        friend SquareMatrix operator-(T lhs, const SquareMatrix& rhs)
        {
            SquareMatrix rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    rslt[i][j] = lhs - rhs[i][j];
                }
            }
            return rslt;
        }

        friend SquareMatrix& operator-=(SquareMatrix& lhs, T rhs)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    lhs[i][j] = lhs[i][j] - rhs;
                }
            }
            return lhs;
        }

        friend std::ostream& operator<<(std::ostream& os, const SquareMatrix& val)
        {
            for (size_t i = 0; i < SIZE; ++i)
            {
                os << "|";
                for (size_t j = 0; j < SIZE; ++j)
                {
                    static constexpr int ColLength{10};
                    if (j != SIZE - 1)
                    {

                        os << fmt::format("{:>{}.2f},", val[j][i], ColLength);
                    }
                    else
                    {
                        os << fmt::format("{:>{}.2f}", val[j][i], ColLength);
                    }
                }
                os << "|\n";
            }
            return os;
        }

      protected:
      private:
        T matrix_of_minors(size_t col, size_t row) const
        {
            SquareMatrix<SIZE - 1, T> tmp;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    if (i == col || j == row)
                    {
                        continue;
                    }

                    size_t insertCol          = i > col ? i - 1 : i;
                    size_t insertRow          = j > row ? j - 1 : j;
                    tmp[insertCol][insertRow] = m_Data[i][j];
                }
            }
            return (col + row) % 2 == 0 ? tmp.determinant() : -1 * tmp.determinant();
        }

        SquareMatrix adjunct() const
        {
            SquareMatrix rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    rslt[i][j] = matrix_of_minors(i, j);
                }
            }

            return rslt.transpose();
        }
    };

    template <typename T = float>
    using SquareMatrix3x3 = SquareMatrix<3, T>;
} // namespace Ignosi::Modules