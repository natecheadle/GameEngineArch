#include <StaticMap.hpp>

#include <gtest/gtest.h>

using namespace Ignosi::Modules;

namespace Ignosi::Test
{
    enum alphabet
    {
        a,
        b,
        c,
        d,
        e,
        f,
        g,
        h,
        i,
        j,
        k,
        l,
        m,
        n,
        o,
        p,
        q,
        r,
        s,
        t,
        u,
        v,
        w,
        x,
        y,
        z,
    };

    using MAP = StaticMap<alphabet, alphabet::a, alphabet::z, int>;

    TEST(StaticMap_Tests, ValidateConstruction)
    {
        MAP test_map;
        for (const auto& pair : test_map)
        {
            ASSERT_EQ(0, pair.second);
        }

        test_map = MAP(1);
        for (const auto& pair : test_map)
        {
            ASSERT_EQ(1, pair.second);
        }
    }

    TEST(StaticMap_Tests, ValidateAt)
    {
        MAP test_map;
        int i = 0;
        for (auto& pair : test_map)
        {
            pair.second = static_cast<int>(pair.first);
        }

        ASSERT_EQ(static_cast<int>(alphabet::g), test_map[alphabet::g]);
        ASSERT_EQ(static_cast<int>(alphabet::g), test_map.at(alphabet::g));

        test_map[alphabet::g] = 27;
        ASSERT_EQ(27, test_map[alphabet::g]);

        test_map.at(alphabet::g) = 29;
        ASSERT_EQ(29, test_map[alphabet::g]);
    }
} // namespace Ignosi::Test