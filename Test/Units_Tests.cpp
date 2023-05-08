#include <Units/Degree.hpp>
#include <Units/Radian.hpp>

#include <gtest/gtest.h>

using namespace nate::Modules;

namespace nate::Test
{
    TEST(Units_Tests, ValidateConstruction)
    {
        Degree<float> deg_val(180.0);
        Radian<float> rad_val(M_PI);

        ASSERT_EQ(deg_val, rad_val);
    }

    TEST(Units_Tests, ValidateAddition)
    {
        Degree<float> deg_val(180.0);
        Radian<float> rad_val(M_PI);

        Radian<float> rslt = deg_val + rad_val;
        ASSERT_EQ(rslt, Radian<float>(M_PI * 2.0f));

        deg_val += rad_val;
        ASSERT_EQ(deg_val, Radian<float>(M_PI * 2.0f));
    }

    TEST(Units_Tests, ValidateSubtraction)
    {
        Degree<float> deg_val(180.0);
        Radian<float> rad_val(M_PI);

        Radian<float> rslt = deg_val - rad_val;
        ASSERT_EQ(rslt, Radian<float>(0.0));

        deg_val -= rad_val;
        ASSERT_EQ(deg_val, Radian<float>(0.0));
    }

    TEST(Units_Tests, ValidateDivision)
    {
        Radian<float> rad_val(M_PI);

        Radian<float> rslt = rad_val / 2.0f;
        ASSERT_EQ(rslt, Radian<float>(M_PI_2));

        rad_val /= 2.0f;
        ASSERT_EQ(rad_val, Radian<float>(M_PI_2));
    }

    TEST(Units_Tests, ValidateMultiplication)
    {
        Radian<float> rad_val(M_PI);

        Radian<float> rslt = rad_val * 2.0f;
        ASSERT_EQ(rslt, Radian<float>(M_PI * 2.0f));

        rad_val *= 2.0f;
        ASSERT_EQ(rad_val, Radian<float>(M_PI * 2.0f));
    }
} // namespace nate::Test