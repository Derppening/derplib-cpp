#include <gtest/gtest.h>

#include <derplib/container/circular_queue.h>

#include <array>

namespace {
template<std::size_t Size>
using cq_int = derplib::container::circular_queue<int, Size>;

struct TestClass {
  enum { none, copy, move } init;

  TestClass() : init(none) {}

  TestClass(const TestClass&) : init(copy) {}
  TestClass(TestClass&&) noexcept : init(move) {}

  TestClass& operator=(const TestClass&) {
    init = copy;
    return *this;
  }
  TestClass& operator=(TestClass&&) noexcept {
    init = move;
    return *this;
  }
};

template<std::size_t Size>
using cq_tc = derplib::container::circular_queue<TestClass, Size>;

TEST(CircularQueueTest, DefaultConstructTrivial) {
  cq_int<5> cq;

  EXPECT_TRUE(cq.empty());
  EXPECT_EQ(0, cq.size());
}

TEST(CircularQueueTest, DefaultConstructThrowContracts) {
  cq_int<5> cq;

  EXPECT_THROW(cq.front(), std::runtime_error);
  EXPECT_THROW(cq.back(), std::runtime_error);
  EXPECT_NO_THROW(cq.pop());
}

TEST(CircularQueueTest, CopyConstructFromOtherTrivial) {
  cq_int<5> orig(std::array<int, 5>{1, 2, 3, 4, 5});
  cq_int<5> actual(orig);

  EXPECT_NE(std::addressof(orig.front()), std::addressof(actual.front()));

  EXPECT_EQ(orig.empty(), actual.empty());
  EXPECT_EQ(orig.size(), actual.size());
  EXPECT_EQ(orig.front(), actual.front());
  EXPECT_EQ(orig.back(), actual.back());
}

TEST(CircularQueueTest, MoveConstructFromOtherTrivial) {
  cq_int<5> orig(std::array<int, 5>{1, 2, 3, 4, 5});
  cq_int<5> actual(std::move(orig));

  EXPECT_EQ(false, actual.empty());
  EXPECT_EQ(5, actual.size());
  EXPECT_EQ(1, actual.front());
  EXPECT_EQ(5, actual.back());
}

TEST(CircularQueueTest, CopyConstructFromOther) {
  cq_tc<5> orig(std::array<TestClass, 5>{});
  cq_tc<5> actual(orig);

  EXPECT_NE(std::addressof(orig.front()), std::addressof(actual.front()));

  EXPECT_EQ(orig.empty(), actual.empty());
  EXPECT_EQ(orig.size(), actual.size());

  EXPECT_EQ(TestClass::copy, actual.front().init);
  EXPECT_EQ(TestClass::copy, actual.back().init);
}

TEST(CircularQueueTest, MoveConstructFromOther) {
  cq_tc<5> orig(std::array<TestClass, 5>{});
  cq_tc<5> actual(std::move(orig));

  EXPECT_EQ(false, actual.empty());
  EXPECT_EQ(5, actual.size());

  EXPECT_EQ(TestClass::move, actual.front().init);
  EXPECT_EQ(TestClass::move, actual.back().init);
}

TEST(CircularQueueTest, CopyConstructFromTrivialArrayOverfill) {
  std::array<int, 7> arr = {{1, 2, 3, 4, 5, 6, 7}};
  cq_int<5> cq(arr);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(5, cq.size());
  EXPECT_EQ(arr.front(), cq.front());
  EXPECT_EQ(arr[4], cq.back());
}

TEST(CircularQueueTest, CopyConstructFromTrivialArrayFull) {
  std::array<int, 5> arr = {{1, 2, 3, 4, 5}};
  cq_int<5> cq(arr);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(arr.size(), cq.size());
  EXPECT_EQ(arr.front(), cq.front());
  EXPECT_EQ(arr.back(), cq.back());
}

TEST(CircularQueueTest, CopyConstructFromTrivialArrayPartial) {
  std::array<int, 3> arr = {{1, 2, 3}};
  cq_int<5> cq(arr);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(arr.size(), cq.size());
  EXPECT_EQ(arr.front(), cq.front());
  EXPECT_EQ(arr.back(), cq.back());
}

TEST(CircularQueueTest, MoveConstructFromTrivialArrayOverfill) {
  std::array<int, 7> arr = {{1, 2, 3, 4, 5, 6, 7}};
  cq_int<5> cq(std::move(arr));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(5, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(5, cq.back());
}

TEST(CircularQueueTest, MoveConstructFromTrivialArrayFull) {
  std::array<int, 5> arr = {{1, 2, 3, 4, 5}};
  cq_int<5> cq(std::move(arr));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(5, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(5, cq.back());
}

TEST(CircularQueueTest, MoveConstructFromTrivialArrayPartial) {
  std::array<int, 3> arr = {{1, 2, 3}};
  cq_int<5> cq(std::move(arr));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(3, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(3, cq.back());
}

TEST(CircularQueueTest, CopyConstructFromArrayOverfill) {
  std::array<TestClass, 7> arr = {};
  cq_tc<5> cq(arr);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(5, cq.size());

  EXPECT_EQ(TestClass::copy, cq.front().init);
  EXPECT_EQ(TestClass::copy, cq.back().init);
}

TEST(CircularQueueTest, CopyConstructFromArrayFull) {
  std::array<TestClass, 5> arr = {};
  cq_tc<5> cq(arr);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(arr.size(), cq.size());

  EXPECT_EQ(TestClass::copy, cq.front().init);
  EXPECT_EQ(TestClass::copy, cq.back().init);
}

TEST(CircularQueueTest, CopyConstructFromArrayPartial) {
  std::array<TestClass, 3> arr = {};
  cq_tc<5> cq(arr);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(arr.size(), cq.size());

  EXPECT_EQ(TestClass::copy, cq.front().init);
  EXPECT_EQ(TestClass::copy, cq.back().init);
}

TEST(CircularQueueTest, MoveConstructFromArrayFull) {
  std::array<TestClass, 5> arr = {};
  cq_tc<5> cq(std::move(arr));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(5, cq.size());

  EXPECT_EQ(TestClass::move, cq.front().init);
  EXPECT_EQ(TestClass::move, cq.back().init);
}

TEST(CircularQueueTest, PushPopOrdering) {
  constexpr std::array<int, 5> elems = {1, 2, 3, 4, 5};

  cq_int<5> cq;
  for (const int e : elems) {
    EXPECT_NO_THROW(cq.push(e)) << "at element " << e;
  }

  EXPECT_EQ(elems.size(), cq.size());

  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(elems[i], cq.front()) << "at index [" << i << "]";
    cq.pop();
  }

  EXPECT_TRUE(cq.empty()) << "has " << cq.size() << " elements";
}

TEST(CircularQueueTest, PushPopWithWraparound) {
  constexpr std::array<int, 5> init_elems = {1, 2, 3, 4, 5};
  constexpr std::array<int, 5> circ_elems = {6, 7, 8, 9, 10};

  cq_int<5> cq(init_elems);
  ASSERT_EQ(init_elems.size(), cq.size());

  // Pass 1: Replace original elements with circ1_elems
  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(init_elems[i], cq.front()) << "at index [" << i << "]";
    cq.pop();
    EXPECT_NO_THROW(cq.push(circ_elems[i])) << "at value " << circ_elems[i] << " with index [" << i << "]";
  }

  EXPECT_EQ(5, cq.size());
  EXPECT_EQ(6, cq.front());
  EXPECT_EQ(10, cq.back());

  // Pass 2: Replace circ1_elems with circ2_elems
  // This is to ensure that after replacing all elements in the circular queue, the queue is in a well-defined state.
  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(circ_elems[i], cq.front()) << "at index [" << i << "]";
    cq.pop();
    EXPECT_NO_THROW(cq.push(init_elems[i])) << "at value " << init_elems[i] << " with index [" << i << "]";
  }

  EXPECT_EQ(5, cq.size());
  EXPECT_EQ(init_elems.front(), cq.front());
  EXPECT_EQ(init_elems.back(), cq.back());

  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(init_elems[i], cq.front()) << "at index [" << i << "]";
    cq.pop();
  }

  EXPECT_TRUE(cq.empty()) << "has " << cq.size() << " elements";
}

TEST(CircularQueueTest, CopyPushElementTrivialWhenEmpty) {
  cq_int<1> cq;

  ASSERT_TRUE(cq.empty());

  cq.push(1);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(1, cq.back());
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, CopyPushElementWhenEmpty) {
  cq_tc<1> cq;

  ASSERT_TRUE(cq.empty());

  cq.push(static_cast<const TestClass&>(TestClass()));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(TestClass::copy, cq.front().init);
  EXPECT_EQ(TestClass::copy, cq.back().init);
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, MovePushElementTrivialWhenEmpty) {
  cq_int<1> cq;

  ASSERT_TRUE(cq.empty());

  cq.push(std::move(1));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(1, cq.back());
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, MovePushElementWhenEmpty) {
  cq_tc<1> cq;

  ASSERT_TRUE(cq.empty());

  cq.push(std::move(TestClass()));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(TestClass::move, cq.front().init);
  EXPECT_EQ(TestClass::move, cq.back().init);
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, CopyPushElementTrivialWhenNonEmpty) {
  cq_int<2> cq(std::array<int, 1>{1});

  ASSERT_FALSE(cq.empty());
  ASSERT_EQ(1, cq.size());

  cq.push(2);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(2, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(2, cq.back());
  EXPECT_NE(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, CopyPushElementWhenNonEmpty) {
  cq_tc<2> cq((std::array<TestClass, 1>()));

  ASSERT_FALSE(cq.empty());

  cq.push(static_cast<const TestClass&>(TestClass()));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(2, cq.size());
  EXPECT_EQ(TestClass::copy, cq.back().init);
  EXPECT_NE(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, MovePushElementTrivialWhenNonEmpty) {
  cq_int<2> cq(std::array<int, 1>{1});

  ASSERT_FALSE(cq.empty());
  ASSERT_EQ(1, cq.size());

  cq.push(std::move(2));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(2, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(2, cq.back());
  EXPECT_NE(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, MovePushElementWhenNonEmpty) {
  cq_tc<2> cq((std::array<TestClass, 1>()));

  ASSERT_FALSE(cq.empty());

  cq.push(std::move(TestClass()));

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(2, cq.size());
  EXPECT_EQ(TestClass::move, cq.back().init);
  EXPECT_NE(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, CopyPushElementTrivialWhenFull) {
  cq_int<1> cq(std::array<int, 1>{1});

  ASSERT_FALSE(cq.empty());
  ASSERT_EQ(1, cq.size());

  ASSERT_THROW(cq.push(2), std::length_error);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(1, cq.back());
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, CopyPushElementWhenFull) {
  cq_tc<1> cq((std::array<TestClass, 1>()));

  ASSERT_FALSE(cq.empty());
  ASSERT_EQ(1, cq.size());

  ASSERT_THROW(cq.push(static_cast<const TestClass&>(TestClass())), std::length_error);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, MovePushElementTrivialFull) {
  cq_int<1> cq(std::array<int, 1>{1});

  ASSERT_FALSE(cq.empty());
  ASSERT_EQ(1, cq.size());

  ASSERT_THROW(cq.push(std::move(2)), std::length_error);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(1, cq.front());
  EXPECT_EQ(1, cq.back());
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

TEST(CircularQueueTest, MovePushElementFull) {
  cq_tc<1> cq((std::array<TestClass, 1>()));

  ASSERT_FALSE(cq.empty());
  ASSERT_EQ(1, cq.size());

  ASSERT_THROW(cq.push(std::move(TestClass())), std::length_error);

  EXPECT_FALSE(cq.empty());
  EXPECT_EQ(1, cq.size());
  EXPECT_EQ(std::addressof(cq.front()), std::addressof(cq.back()));
}

}  // namespace
