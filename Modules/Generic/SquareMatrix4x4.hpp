#include "SquareMatrix.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#include <stdexcept>

namespace nate::Modules
{
    template <typename T = float>
    class SquareMatrix4x4 : SquareMatrix<4, T>
    {
      public:
        SquareMatrix4x4() = default;

        SquareMatrix4x4(std::array<Vector<4, T>, 4> init)
            : SquareMatrix<4, T>(init)
        {
        }

        template <typename IT>
        SquareMatrix4x4(IT begin, IT end)
            : SquareMatrix<4, T>(begin, end)
        {
        }

        SquareMatrix4x4(const SquareMatrix4x4& other) = default;
        SquareMatrix4x4(SquareMatrix4x4&& other)      = default;

        SquareMatrix4x4& operator=(const SquareMatrix4x4& other) = default;
        SquareMatrix4x4& operator=(SquareMatrix4x4&& other)      = default;

        SquareMatrix4x4 scale(const Vector3<T>& scale) const
        {
            SquareMatrix4x4 rslt = SquareMatrix<4, T>::identity();
            rslt[0][0]           = scale[0];
            rslt[1][1]           = scale[1];
            rslt[2][2]           = scale[2];

            rslt *= *this;
            return rslt;
        }

        SquareMatrix4x4 translate(const Vector3<T>& trans) const
        {
            SquareMatrix4x4 rslt = SquareMatrix<4, T>::identity();
            rslt[3]              = Vector4<T>(trans);
            rslt *= *this;
            return rslt;
        }

        SquareMatrix4x4 rotate(const Vector3<T>& rotAxis, T theta) const
        {
            if (std::abs(rotAxis.length() - 1.0) > 1e-9)
            {
                throw std::invalid_argument("Rotation Axis must be a unit vector.");
            }

            SquareMatrix4x4 rslt;
            rslt[0][0] = std::cos(theta) + rotAxis.X() * rotAxis.X() * (1 - std::cos(theta));
            rslt[1][0] = rotAxis.X() * rotAxis.Y() * (1 - std::cos(theta)) - rotAxis.Z() * std::sin(theta);
            rslt[2][0] = rotAxis.X() * rotAxis.Z() * (1 - std::cos(theta)) + rotAxis.Y() * std::sin(theta);

            rslt[0][1] = rotAxis.Y() * rotAxis.X() * (1 - std::cos(theta)) + rotAxis.Z() * std::sin(theta);
            rslt[1][1] = std::cos(theta) + rotAxis.Y() * rotAxis.Y() * (1 - std::cos(theta));
            rslt[2][1] = rotAxis.Y() * rotAxis.Z() * (1 - std::cos(theta)) - rotAxis.X() * std::sin(theta);

            rslt[0][2] = rotAxis.Z() * rotAxis.X() * (1 - std::cos(theta)) - rotAxis.Y() * std::sin(theta);
            rslt[1][2] = rotAxis.Z() * rotAxis.Y() * (1 - std::cos(theta)) + rotAxis.X() * std::sin(theta);
            rslt[2][2] = std::cos(theta) + rotAxis.Z() * rotAxis.Z() * (1 - std::cos(theta));

            rslt *= *this;
            return rslt;
        }

        SquareMatrix4x4 rotate_x(T theta) const
        {
            SquareMatrix4x4 rslt = SquareMatrix<4, T>::identity();
            rslt[1][1]           = std::cos(theta);
            rslt[2][1]           = -std::sin(theta);
            rslt[1][2]           = std::sin(theta);
            rslt[2][2]           = std::cos(theta);

            rslt *= *this;
            return rslt;
        }

        SquareMatrix4x4 rotate_y(T theta) const
        {
            SquareMatrix4x4 rslt = SquareMatrix<4, T>::identity();
            rslt[0][0]           = std::cos(theta);
            rslt[0][2]           = -std::sin(theta);
            rslt[2][0]           = std::sin(theta);
            rslt[2][2]           = std::cos(theta);

            rslt *= *this;
            return rslt;
        }

        SquareMatrix4x4 rotate_z(T theta) const
        {
            SquareMatrix4x4 rslt = SquareMatrix<4, T>::identity();
            rslt[0][0]           = std::cos(theta);
            rslt[1][0]           = -std::sin(theta);
            rslt[0][1]           = std::sin(theta);
            rslt[1][1]           = std::cos(theta);

            rslt *= *this;
            return rslt;
        }
    };
} // namespace nate::Modules