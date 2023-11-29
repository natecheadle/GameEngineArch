#include <gtest/gtest.h>

#include <math_lib.h>

namespace Ignosi::Test {

TEST(MathLibFixture, AddSuccessful) {
  ASSERT_DOUBLE_EQ(Libraries::Add(2, 2), 4);
}

TEST(MathLibFixture, SubtractSuccessful) {
  ASSERT_DOUBLE_EQ(Libraries::Subtract(5, 2), 3);
}

} // namespace Ignosi::Test
