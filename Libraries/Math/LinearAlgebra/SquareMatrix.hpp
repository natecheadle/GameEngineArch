#pragma once

#include "Units/Radian.hpp"
#include "Vector.hpp"

#include <array>
#include <cstddef>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace Ignosi::Libraries::Math
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

        static SquareMatrix identity()
        {
            SquareMatrix rslt;
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

        static SquareMatrix lookat(const Vector3<T>& pos, const Vector3<T>& tar, const Vector3<T>& up)
        {
            static_assert(SIZE == 4, "lookat requires a 4x4 square matrix");

            Vector3<T> dir = pos - tar;
            dir.normalize_this();
            Vector3<T> right  = up.cross(dir).normalize_this();
            Vector3<T> cam_up = dir.cross(right);

            SquareMatrix lookat(identity());
            lookat[0] = right.ToVector4(0.0f);
            lookat[1] = cam_up.ToVector4(0.0f);
            lookat[2] = dir.ToVector4(0.0f);
            lookat.transpose_this();

            SquareMatrix posMat(identity());
            posMat[3] = (-1 * pos).ToVector4(1.0f);

            lookat *= posMat;
            return lookat;
        }

        static SquareMatrix perspective(Radian<T> fov, T aspect, T near, T far)
        {
            static_assert(SIZE == 4, "perspective requires a 4x4 square matrix");

            SquareMatrix per;
            T            ta = tan(fov / T(2.0));

            per[0][0] = 1 / (ta * aspect);
            per[1][1] = 1 / ta;
            per[2][2] = -(far + near) / (far - near);
            per[3][2] = (-2 * far * near) / (far - near);
            per[2][3] = -1;

            return per;
        }

        static SquareMatrix orthographic(T left, T right, T bottom, T top, T near, T far)
        {
            static_assert(SIZE == 4, "orthographic requires a 4x4 square matrix");

            SquareMatrix ortho;
            ortho[0][0] = T(2) / (right - left);
            ortho[1][1] = T(2) / (top - bottom);
            ortho[2][2] = T(-2) / (far - near);
            ortho[3][0] = T(-1) * (right + left) / (right - left);
            ortho[3][1] = T(-1) * (top + bottom) / (top - bottom);
            ortho[3][2] = T(-1) * (far + near) / (far - near);
            ortho[3][3] = T(1);

            return ortho;
        }

        static SquareMatrix scale_init(const Vector3<T>& scale)
        {
            static_assert(SIZE == 4, "scale_init requires a 4x4 square matrix");

            SquareMatrix rslt = identity();
            rslt[0][0]        = scale[0];
            rslt[1][1]        = scale[1];
            rslt[2][2]        = scale[2];

            return rslt;
        }

        static SquareMatrix translate_init(const Vector3<T>& trans)
        {
            static_assert(SIZE == 4, "translate_init requires a 4x4 square matrix");

            SquareMatrix rslt = identity();
            rslt[3]           = trans.ToVector4(1.0f);

            return rslt;
        }

        static SquareMatrix rotate_init(const Vector3<T>& rotAxis, Radian<T> theta)
        {
            static_assert(SIZE == 4, "rotate_init requires a 4x4 square matrix");

            auto createMatrix = [](const Vector3<T>& rotAxis, Radian<T> theta) -> SquareMatrix {
                SquareMatrix rslt;
                rslt[0][0] = cos(theta) + rotAxis.X() * rotAxis.X() * (1 - cos(theta));
                rslt[1][0] = rotAxis.X() * rotAxis.Y() * (1 - cos(theta)) - rotAxis.Z() * sin(theta);
                rslt[2][0] = rotAxis.X() * rotAxis.Z() * (1 - cos(theta)) + rotAxis.Y() * sin(theta);

                rslt[0][1] = rotAxis.Y() * rotAxis.X() * (1 - cos(theta)) + rotAxis.Z() * sin(theta);
                rslt[1][1] = cos(theta) + rotAxis.Y() * rotAxis.Y() * (1 - cos(theta));
                rslt[2][1] = rotAxis.Y() * rotAxis.Z() * (1 - cos(theta)) - rotAxis.X() * sin(theta);

                rslt[0][2] = rotAxis.Z() * rotAxis.X() * (1 - cos(theta)) - rotAxis.Y() * sin(theta);
                rslt[1][2] = rotAxis.Z() * rotAxis.Y() * (1 - cos(theta)) + rotAxis.X() * sin(theta);
                rslt[2][2] = cos(theta) + rotAxis.Z() * rotAxis.Z() * (1 - cos(theta));
                return rslt;
            };

            return std::abs(rotAxis.length() - 1.0) > 1e-9 ? createMatrix(rotAxis.normalize(), theta) : createMatrix(rotAxis, theta);
        }

        static SquareMatrix rotate_x_init(Radian<T> theta)
        {
            static_assert(SIZE == 4, "rotate_x_init requires a 4x4 square matrix");

            SquareMatrix rslt = identity();
            rslt[1][1]        = cos(theta);
            rslt[2][1]        = -sin(theta);
            rslt[1][2]        = sin(theta);
            rslt[2][2]        = cos(theta);
            return rslt;
        }

        static SquareMatrix rotate_y_init(Radian<T> theta)
        {
            static_assert(SIZE == 4, "rotate_y_init requires a 4x4 square matrix");

            SquareMatrix rslt = identity();
            rslt[0][0]        = cos(theta);
            rslt[0][2]        = -sin(theta);
            rslt[2][0]        = sin(theta);
            rslt[2][2]        = cos(theta);
            return rslt;
        }

        static SquareMatrix rotate_z_init(Radian<T> theta)
        {
            static_assert(SIZE == 4, "rotate_z_init requires a 4x4 square matrix");

            SquareMatrix rslt = identity();
            rslt[0][0]        = cos(theta);
            rslt[1][0]        = -sin(theta);
            rslt[0][1]        = sin(theta);
            rslt[1][1]        = cos(theta);
            return rslt;
        }

        static SquareMatrix rotate_zyx_init(const Vector3<Radian<T>>& rot)
        {
            static_assert(SIZE == 4, "rotate_zyx_init requires a 4x4 square matrix");

            SquareMatrix rslt  = identity();
            Radian<T>    alpha = rot.z();
            Radian<T>    beta  = rot.y();
            Radian<T>    gamma = rot.x();

            rslt[0][0] = cos(alpha) * cos(beta);
            rslt[0][1] = sin(alpha) * cos(beta);
            rslt[0][2] = -sin(beta);

            rslt[1][0] = cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
            rslt[1][1] = sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
            rslt[1][2] = cos(beta) * sin(gamma);

            rslt[2][0] = cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);
            rslt[2][1] = sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);
            rslt[2][2] = cos(beta) * cos(gamma);

            return rslt;
        }

        SquareMatrix scale(const Vector3<T>& scale) const noexcept
        {
            static_assert(SIZE == 4, "scale requires a 4x4 square matrix");
            SquareMatrix rslt = scale_init(scale);
            rslt              = *this * rslt;
            return rslt;
        }

        SquareMatrix& scale_this(const Vector3<T>& scale) noexcept
        {
            static_assert(SIZE == 4, "scale_this requires a 4x4 square matrix");
            *this *= scale_init(scale);
            return *this;
        }

        SquareMatrix translate(const Vector3<T>& trans) const noexcept
        {
            static_assert(SIZE == 4, "translate requires a 4x4 square matrix");
            SquareMatrix rslt = translate_init(trans);
            rslt              = *this * rslt;
            return rslt;
        }

        SquareMatrix& translate_this(const Vector3<T>& trans) noexcept
        {
            static_assert(SIZE == 4, "translate_this requires a 4x4 square matrix");
            *this *= translate_init(trans);
            return *this;
        }

        SquareMatrix rotate(const Vector3<T>& rotAxis, T theta) const noexcept
        {
            static_assert(SIZE == 4, "rotate requires a 4x4 square matrix");
            SquareMatrix rslt = rotate_init(rotAxis, theta);
            rslt              = *this * rslt;
            return rslt;
        }

        SquareMatrix& rotate_this(const Vector3<T>& rotAxis, T theta) noexcept
        {
            static_assert(SIZE == 4, "rotate_this requires a 4x4 square matrix");
            *this *= rotate_init(rotAxis, theta);
            return *this;
        }

        SquareMatrix rotate_x(T theta) const
        {
            static_assert(SIZE == 4, "rotate_x requires a 4x4 square matrix");
            SquareMatrix rslt = rotate_x_init(theta);
            rslt              = *this * rslt;
            return rslt;
        }

        SquareMatrix& rotate_x_this(T theta)
        {
            static_assert(SIZE == 4, "rotate_x_this requires a 4x4 square matrix");
            *this *= rotate_x_init(theta);
            return *this;
        }

        SquareMatrix rotate_y(T theta) const
        {
            static_assert(SIZE == 4, "rotate_y requires a 4x4 square matrix");
            SquareMatrix rslt = rotate_y_init(theta);
            rslt              = *this * rslt;
            return rslt;
        }

        SquareMatrix& rotate_y_this(T theta)
        {
            static_assert(SIZE == 4, "rotate_y_this requires a 4x4 square matrix");
            *this *= rotate_y_init(theta);
            return *this;
        }

        SquareMatrix rotate_z(T theta) const
        {
            static_assert(SIZE == 4, "rotate_z requires a 4x4 square matrix");
            SquareMatrix rslt = rotate_z_init(theta);
            rslt              = *this * rslt;
            return rslt;
        }

        SquareMatrix& rotate_z_this(T theta)
        {
            static_assert(SIZE == 4, "rotate_z_this requires a 4x4 square matrix");
            *this *= rotate_z_init(theta);
            return *this;
        }

        SquareMatrix<3, T> to_3x3() const
        {
            static_assert(SIZE == 4, "to_3x3 requires a 4x4 square matrix");
            SquareMatrix<3, T> rslt;
            for (size_t i = 0; i < 3; ++i)
            {
                const Vector<4, T>& col = this->at(i);
                for (size_t j = 0; j < 3; ++j)
                {
                    rslt[i][j] = col[j];
                }
            }
            return rslt;
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

        friend Vector<SIZE, T> operator*(const SquareMatrix<SIZE, T>& lhs, const Vector<SIZE, T>& rhs)
        {
            Vector<SIZE, T> rslt;
            for (size_t i = 0; i < SIZE; ++i)
            {
                for (size_t j = 0; j < SIZE; ++j)
                {
                    rslt[i] += lhs[j][i] * rhs[i];
                }
            }
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

    template <typename T = float>
    using SquareMatrix4x4 = SquareMatrix<4, T>;
} // namespace Ignosi::Libraries::Math
