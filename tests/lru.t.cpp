#include <gtest/gtest.h>
#include <lru.h>

namespace test {
using namespace ::testing;
using namespace pylru;

TEST(pylru_lru, simple_test) {
    lru<int, int> cache(2);
    cache.put(1, 1);
    cache.put(2, 2);

    auto result1 = cache.get(1);
    EXPECT_TRUE(result1.is_found());
    EXPECT_EQ(result1.value(), 1);

    cache.put(3, 3);
    auto result2 = cache.get(2);
    EXPECT_TRUE(result2.is_not_found());
    EXPECT_EQ(result2.status(), pylru::Status::NOT_FOUND);

    cache.put(4, 4);
    auto result3 = cache.get(1);
    EXPECT_TRUE(result3.is_not_found());

    auto result4 = cache.get(3);
    EXPECT_TRUE(result4.is_found());
    EXPECT_EQ(result4.value(), 3);

    auto result5 = cache.get(4);
    EXPECT_TRUE(result5.is_found());
    EXPECT_EQ(result5.value(), 4);
}
} // namespace test
