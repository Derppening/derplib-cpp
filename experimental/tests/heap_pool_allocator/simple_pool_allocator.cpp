#include <gtest/gtest.h>

#include <derplib/experimental/heap_pool_allocator/simple_pool_allocator.h>

namespace {
using derplib::experimental::simple_pool_allocator;

TEST(SimplePoolAllocatorTest, ZeroSizeCaseTest) {
  constexpr std::size_t size{0};

  // Construction Test
  std::unique_ptr<simple_pool_allocator> allocator{};
  EXPECT_NO_THROW((allocator = std::unique_ptr<simple_pool_allocator>(new simple_pool_allocator(size, {}))));

  // Allocate Test
  EXPECT_EQ(nullptr, allocator->allocate(1));
}

TEST(SimplePoolAllocatorTest, ConstructDestructTest) {
  constexpr std::size_t size{sizeof(int)};

  simple_pool_allocator allocator{size, {}};
}

TEST(SimplePoolAllocatorTest, SingleFullAllocTest) {
  constexpr std::size_t size{sizeof(int)};

  simple_pool_allocator allocator{size, {}};

  // Test Action: Allocation of sizeof(int) bytes
  ASSERT_NE(nullptr, allocator.allocate(sizeof(int)));

  // Test Condition: Heap Pool is full
  EXPECT_EQ(nullptr, allocator.allocate(1));
}

TEST(SimplePoolAllocatorTest, SingleFullDeallocTest) {
  constexpr std::size_t size{sizeof(int)};

  simple_pool_allocator allocator{size, {}};

  // Precondition: Allocate an element to fill the pool
  void* ptr;
  ASSERT_NE(nullptr, (ptr = allocator.allocate(sizeof(int))));
  ASSERT_EQ(nullptr, allocator.allocate(1)) << "precondition failed: space remaining after heap full";

  // Test Action: Deallocate the only element in the pool
  allocator.deallocate(ptr);

  // Test Condition: Heap Pool is empty
  EXPECT_NE(nullptr, allocator.allocate(4));
}

TEST(SimplePoolAllocatorTest, FullAllocTest) {
  constexpr std::size_t allocs{2};
  constexpr std::size_t size{allocs * sizeof(int)};

  simple_pool_allocator allocator{size, {}};

  // Test Action: Allocation of 2x4 bytes
  std::array<void*, allocs> alloc_address{};
  for (std::size_t i{0}; i < allocs; ++i) {
    EXPECT_NE(nullptr, (alloc_address[i] = allocator.allocate(sizeof(int))));
  }

  // Test Condition: Heap Pool is full
  EXPECT_EQ(nullptr, allocator.allocate(1));
  // Test Condition: Two allocations are sizeof(int) bytes apart
  EXPECT_EQ(sizeof(int), reinterpret_cast<char*>(alloc_address[1]) - reinterpret_cast<char*>(alloc_address[0]));
}

TEST(SimplePoolAllocatorTest, FullDeallocTest) {
  constexpr std::size_t allocs{2};
  constexpr std::size_t size{allocs * sizeof(int)};

  simple_pool_allocator allocator{size, {}};

  // Precondition: Heap Pool is full
  std::array<void*, allocs> alloc_address{};
  for (std::size_t i{0}; i < allocs; ++i) {
    ASSERT_NE(nullptr, (alloc_address[i] = allocator.allocate(sizeof(int))));
  }
  ASSERT_EQ(nullptr, allocator.allocate(1)) << "precondition failed: space remaining after heap full";

  // Test Action: Deallocate all elements
  for (void* ptr : alloc_address) {
    allocator.deallocate(ptr);
  }

  void* new_alloc{};
  // Test Condition: Allocation for entire heap pool is successful
  EXPECT_NE(nullptr, (new_alloc = allocator.allocate(size)));
  // Test Condition: Allocation is performed at the start of the heap pool
  EXPECT_EQ(alloc_address[0], new_alloc);
}


TEST(SimplePoolAllocatorTest, SingleAlignedAllocTest) {
  using integer_type = std::uint32_t;

  constexpr std::size_t size{2 * sizeof(integer_type)};

  simple_pool_allocator allocator{size, {}};

  // Precondition: Allocate a char such that we have [1B used; 7B unused] in the pool
  void* cptr{};
  EXPECT_NE(nullptr, (cptr = allocator.allocate(1)));

  // Test Action: Allocation of 4 bytes with 4B alignment
  void* iptr{};
  EXPECT_NE(nullptr, (iptr = allocator.allocate(sizeof(integer_type), alignof(integer_type))));

  // Test Condition: Allocation of 4B is allocated at alignment boundaries
  //                 i.e. [1B [char] used; 3B unused; 4B [uint32_t] used]
  EXPECT_EQ(alignof(integer_type), reinterpret_cast<char*>(iptr) - reinterpret_cast<char*>(cptr));
}
}  // namespace
