#include <SquareMatrix.hpp>
#include <Vector.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>

#include <gtest/gtest.h>

using namespace nate::Modules;

namespace nate::Test
{
    SquareMatrix4x4<float> test{
        {{{{1.0, 2.0, 3.0, 4.0}}, {{1.0, 2.0, 3.0, 4.0}}, {{1.0, 2.0, 3.0, 4.0}}, {{1.0, 2.0, 3.0, 4.0}}}}};

    TEST(MatrixTests, ValidateDeterminate)
    {
        ASSERT_EQ(0, test.determinant());
    }

    TEST(MatrixTests, ValidateAddition)
    {
        SquareMatrix4x4<float> rslt{
            {{{{2.0, 4.0, 6.0, 8.0}}, {{2.0, 4.0, 6.0, 8.0}}, {{2.0, 4.0, 6.0, 8.0}}, {{2.0, 4.0, 6.0, 8.0}}}}};

        ASSERT_EQ(rslt, test + test);
    }

    TEST(MatrixTests, ValidateSubtraction)
    {
        SquareMatrix4x4<float> rslt{
            {{{{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}}}};

        ASSERT_EQ(rslt, test - test);
    }

    TEST(MatrixTests, ValidateMultiplication)
    {
        SquareMatrix4x4<float> rslt{
            {{{{10.0, 20.0, 30.0, 40.0}},
              {{10.0, 20.0, 30.0, 40.0}},
              {{10.0, 20.0, 30.0, 40.0}},
              {{10.0, 20.0, 30.0, 40.0}}}}};

        ASSERT_EQ(rslt, test * test);
    }

    TEST(MatrixTests, ValidateIdentity)
    {
        SquareMatrix4x4<float> rslt{
            {{{{1.0, 0.0, 0.0, 0.0}}, {{0.0, 1.0, 0.0, 0.0}}, {{0.0, 0.0, 1.0, 0.0}}, {{0.0, 0.0, 0.0, 1.0}}}}};
        ASSERT_EQ(rslt, SquareMatrix4x4<float>::identity());
    }

    TEST(MatrixTests, ValidateInverse)
    {
        SquareMatrix3x3<float> input{{{{{2.0, 1.0, -1.0}}, {{-3.0, -1.0, 2.0}}, {{-2.0, 1.0, 2.0}}}}};
        SquareMatrix3x3<float> rslt{{{{{4.0, 3.0, -1.0}}, {{-2.0, -2.0, 1.0}}, {{5.0, 4.0, -1.0}}}}};
        ASSERT_EQ(rslt, input.invert());
    }
} // namespace nate::Test