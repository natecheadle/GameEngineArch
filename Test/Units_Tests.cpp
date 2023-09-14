#include <Units/Degree.hpp>
#include <Units/Radian.hpp>

#include <gtest/gtest.h>

#include <numbers>

using namespace Ignosi::Modules;

namespace Ignosi::Test
{
    TEST(Units_Tests, ValidateConstruction)
    {
        Degree<float> deg_val(180.0);
        Radian<float> rad_val(std::numbers::pi_v<float>);

        ASSERT_EQ(deg_val, rad_val);
    }

    TEST(Units_Tests, ValidateAddition)
    {
        Degree<float> deg_val(180.0);
        Radian<float> rad_val(std::numbers::pi_v<float>);

        Radian<float> rslt = deg_val + rad_val;
        ASSERT_EQ(rslt, Radian<float>(std::numbers::pi_v<float> * 2.0f));

        deg_val += rad_val;
        ASSERT_EQ(deg_val, Radian<float>(std::numbers::pi_v<float> * 2.0f));
    }

    TEST(Units_Tests, ValidateSubtraction)
    {
        Degree<float> deg_val(180.0);
        Radian<float> rad_val(std::numbers::pi_v<float>);

        Radian<float> rslt = deg_val - rad_val;
        ASSERT_EQ(rslt, Radian<float>(0.0));

        deg_val -= rad_val;
        ASSERT_EQ(deg_val, Radian<float>(0.0));
    }

    TEST(Units_Tests, ValidateDivision)
    {
        Radian<float> rad_val(std::numbers::pi_v<float>);

        Radian<float> rslt = rad_val / 2.0f;
        ASSERT_EQ(rslt, Radian<float>(std::numbers::pi_v<float> / 2.0f));

        rad_val /= 2.0f;
        ASSERT_EQ(rad_val, Radian<float>(std::numbers::pi_v<float> / 2.0f));
    }

    TEST(Units_Tests, ValidateMultiplication)
    {
        Radian<float> rad_val(std::numbers::pi_v<float>);

        Radian<float> rslt = rad_val * 2.0f;
        ASSERT_EQ(rslt, Radian<float>(std::numbers::pi_v<float> * 2.0f));

        rad_val *= 2.0f;
        ASSERT_EQ(rad_val, Radian<float>(std::numbers::pi_v<float> * 2.0f));
    }

    TEST(Units_Tests, ValidateComparison)
    {
        Radian<float> rad_val(std::numbers::pi_v<float>);
        Degree<float> deg_val(180.0);
        Radian<float> large_val(std::numbers::pi_v<float> + std::numbers::pi_v<float> / 2.0f);
        Radian<float> small_val(std::numbers::pi_v<float> / 2.0f);

        ASSERT_EQ(rad_val, deg_val);
        ASSERT_LT(small_val, rad_val);
        ASSERT_GT(large_val, rad_val);
        ASSERT_LT(small_val, deg_val);
        ASSERT_GT(large_val, deg_val);
    }
} // namespace Ignosi::Test