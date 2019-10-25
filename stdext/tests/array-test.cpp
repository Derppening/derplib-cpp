#include <gtest/gtest.h>

#include <derplib/stdext/array.h>

namespace {
namespace stdext = derplib::stdext;

struct ToArrayClass {
  ToArrayClass() = default;

  ToArrayClass(const ToArrayClass&) { ++ToArrayClass::copies; }
  ToArrayClass(ToArrayClass&&) noexcept { ++ToArrayClass::moves; }

  ToArrayClass& operator=(const ToArrayClass&) { ++ToArrayClass::copies; return *this; }
  ToArrayClass& operator=(ToArrayClass&&) noexcept { ++ToArrayClass::moves; return *this; }

  std::size_t copies = 0;
  std::size_t moves = 0;
};

TEST(ArrayTest, ToArrayMove) {
  ToArrayClass given[1];
  auto actual = stdext::to_array(std::move(given));

  EXPECT_EQ(1, actual.front().moves) << "Mismatching number of moves";
  EXPECT_EQ(0, actual.front().copies) << "Mismatching number of copies";
}

TEST(ArrayTest, ToArrayCopy) {
  ToArrayClass given[1];
  auto actual = stdext::to_array(given);

  EXPECT_EQ(0, actual.front().moves) << "Mismatching number of moves";
  EXPECT_EQ(1, actual.front().copies) << "Mismatching number of copies";
}

TEST(ArrayTest, ToArrayTrivialMove) {
  int given[5] = {1, 2, 3, 4, 5};
  auto actual = stdext::to_array(std::move(given));

  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(i + 1, actual[i]) << "at array element [" << i << "]";
  }
}

TEST(ArrayTest, ToArrayTrivialCopy) {
  int given[5] = {1, 2, 3, 4, 5};
  auto actual = stdext::to_array(given);

  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(given[i], actual[i]) << "at array element [" << i << "]";
  }
}

TEST(ArrayTest, ArraySize) {
  constexpr std::size_t expected = 5;

  int given[expected] = {};
  auto actual = stdext::size(given);

  EXPECT_EQ(expected, actual);
}
}  // namespace
