#include <gtest/gtest.h>

#include <sstream>
#include <derplib/util/iterator.h>

namespace {
namespace util = derplib::util;

TEST(IteratorTest, ForEachIndexedImmutable) {
  std::ostringstream actual_oss;

  auto func = [&actual_oss](std::size_t i, int val) {
    actual_oss << i << val << ' ';
  };

  const std::array<int, 5> given = {1, 2, 3, 4, 5};
  std::array<int, 5> actual = given;
  ASSERT_EQ(given.size(), actual.size()) << "Should never occur!";

  util::for_each_indexed(actual.begin(), actual.end(), func);
  EXPECT_EQ(given, actual) << "Immutable contract broken";

  std::ostringstream expected_oss;
  for (std::size_t i = 0; i < given.size(); ++i) {
    expected_oss << i << actual[i] << ' ';
  }
  EXPECT_EQ(expected_oss.str(), actual_oss.str());
}

TEST(IteratorTest, ForEachIndexedMutable) {
  auto func = [](std::size_t i, int& val) {
    val += i;
  };

  const std::array<int, 5> given = {1, 2, 3, 4, 5};
  std::array<int, 5> actual = given;
  ASSERT_EQ(given.size(), actual.size()) << "Should never occur!";

  util::for_each_indexed(actual.begin(), actual.end(), func);

  for (std::size_t i = 0; i < given.size(); ++i) {
    EXPECT_EQ(given[i] + i, actual[i]) << "at index [" << i << "]";
  }
}

}  // namespace
