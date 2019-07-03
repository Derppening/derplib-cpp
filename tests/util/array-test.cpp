#include <gtest/gtest.h>

#include <derplib/util/array.h>

namespace {
namespace util = derplib::util;

TEST(ArrayTest, ToArrayInt) {
  int given[5] = {1, 2, 3, 4, 5};
  auto actual = util::to_array(given);

  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(given[i], actual[i]) << "at array element [" << i << "]";
  }
}

TEST(ArrayTest, ArraySize) {
  constexpr std::size_t expected = 5;

  int given[expected] = {};
  auto actual = util::size(given);

  EXPECT_EQ(expected, actual);
}
}  // namespace
