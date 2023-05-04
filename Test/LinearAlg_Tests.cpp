#include <SquareMatrix.hpp>
#include <SquareMatrix4x4.hpp>
#include <Vector.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>

#include <gtest/gtest.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace nate::Modules;

namespace nate::Test
{
    class MatrixTests : public testing::Test
    {
      public:
        SquareMatrix4x4<float> test;
        MatrixTests()
            : test({{{{{1.0, 2.0, 3.0, 4.0}}, {{1.0, 2.0, 3.0, 4.0}}, {{1.0, 2.0, 3.0, 4.0}}, {{1.0, 2.0, 3.0, 4.0}}}}})
        {
        }
    };

    TEST_F(MatrixTests, ValidateDeterminate)
    {
        ASSERT_EQ(0, test.determinant());
    }

    TEST_F(MatrixTests, ValidateAddition)
    {
        SquareMatrix4x4<float> rslt{
            {{{{2.0, 4.0, 6.0, 8.0}}, {{2.0, 4.0, 6.0, 8.0}}, {{2.0, 4.0, 6.0, 8.0}}, {{2.0, 4.0, 6.0, 8.0}}}}};

        ASSERT_EQ(rslt, test + test);
    }

    TEST_F(MatrixTests, ValidateSubtraction)
    {
        SquareMatrix4x4<float> rslt{
            {{{{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}}}};

        ASSERT_EQ(rslt, test - test);
    }

    TEST_F(MatrixTests, ValidateMultiplication)
    {
        SquareMatrix4x4<float> rslt{
            {{{{10.0, 20.0, 30.0, 40.0}},
              {{10.0, 20.0, 30.0, 40.0}},
              {{10.0, 20.0, 30.0, 40.0}},
              {{10.0, 20.0, 30.0, 40.0}}}}};

        ASSERT_EQ(rslt, test * test);
    }

    TEST_F(MatrixTests, ValidateIdentity)
    {
        SquareMatrix4x4<float> rslt{
            {{{{1.0, 0.0, 0.0, 0.0}}, {{0.0, 1.0, 0.0, 0.0}}, {{0.0, 0.0, 1.0, 0.0}}, {{0.0, 0.0, 0.0, 1.0}}}}};
        ASSERT_EQ(rslt, SquareMatrix4x4<float>::identity());
    }

    TEST_F(MatrixTests, ValidateInverse)
    {
        SquareMatrix3x3<float> input{{{{{2.0, 1.0, -1.0}}, {{-3.0, -1.0, 2.0}}, {{-2.0, 1.0, 2.0}}}}};
        SquareMatrix3x3<float> rslt{{{{{4.0, 3.0, -1.0}}, {{-2.0, -2.0, 1.0}}, {{5.0, 4.0, -1.0}}}}};
        ASSERT_EQ(rslt, input.invert());
    }

    TEST(VectorTests, ValidateDotProduct)
    {
        Vector3<float> vec({1.0, 2.0, 3.0});
        float          rslt = vec.dot(vec);
        ASSERT_EQ(14, rslt);
    }

    TEST(VectorTests, ValidateCrossProduct)
    {
        Vector3<float> vec({1.0, 2.0, 3.0});
        Vector3<float> rslt = vec.cross(vec);
        ASSERT_EQ(Vector3<float>({0, 0, 0}), rslt);
    }

    TEST(LinearAlg_Tests, ValidateRotateX)
    {
        Vector3<float> init({0, 1, 1});
        Vector3<float> rotated = (SquareMatrix4x4<float>::rotate_x_init(M_PI_4) * init).ToVector3();
        ASSERT_EQ(Vector3({0, 0, std::sqrt(2.0f)}), rotated);
    }

    TEST(LinearAlg_Tests, ValidateRotateY)
    {
        Vector3<float> init({1, 0, 1});
        Vector3<float> rotated = (SquareMatrix4x4<float>::rotate_y_init(M_PI_4) * init).ToVector3();
        ASSERT_EQ(Vector3({std::sqrt(2.0f), 0, 0}), rotated);
    }

    TEST(LinearAlg_Tests, ValidateRotateZ)
    {
        Vector3<float> init({1, 1, 0});
        Vector3<float> rotated = (SquareMatrix4x4<float>::rotate_z_init(M_PI_4) * init).ToVector3();
        ASSERT_EQ(Vector3<float>({0, std::sqrt(2.0f), 0}), rotated);
    }

    TEST(LinearAlg_Tests, ValidateScale)
    {
        Vector3<float> init({1, 1, 1});
        Vector3<float> scaled = (SquareMatrix4x4<float>::scale_init({
                                     {2.0, 2.0, 2.0}
        }) *
                                 init)
                                    .ToVector3();
        ASSERT_EQ(Vector3({2.0, 2.0, 2.0}), scaled);
    }

    TEST(LinearAlg_Tests, ValidateTranslate)
    {
        Vector3 init({1, 1, 1});
        Vector  translated = (SquareMatrix4x4<float>::translate_init({
                                 {2.0, 2.0, 2.0}
        }) *
                             init)
                                .ToVector3();
        ASSERT_EQ(Vector3({3.0, 3.0, 3.0}), translated);
    }
} // namespace nate::Test
