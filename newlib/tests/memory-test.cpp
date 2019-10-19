#include <gtest/gtest.h>

#include "../include/derplib/newlib/memory.h"

#include <derplib/stdext/memory.h>

namespace {
struct A {
  int func() { return 1; }
};

TEST(MemoryTest, FuncInvocationDefault) {
  std::unique_ptr<A> obj = derplib::stdext::make_unique<A>();
  std::unique_ptr<int> actual = derplib::newlib::invoke(obj, &A::func);
  ASSERT_TRUE(actual);
  EXPECT_EQ(1, *actual);
}

TEST(MemoryTest, FuncInvocationNoObject) {
  std::unique_ptr<A> obj;
  std::unique_ptr<int> actual = derplib::newlib::invoke(obj, &A::func);
  EXPECT_FALSE(actual);
}

}  // namespace
