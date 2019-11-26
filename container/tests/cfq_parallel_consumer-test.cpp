#include <gtest/gtest.h>

#include <derplib/container/cfq_parallel_consumer.h>

#include <atomic>
#include <chrono>
#include <functional>

namespace {
template<typename InT, typename ConsumerT = void (*)(InT)>
using cpc = derplib::container::cfq_parallel_consumer<InT, ConsumerT>;

TEST(CFQParallelConsumerTest, CheckConcurrency) {
  std::mutex mutex;
  std::condition_variable_any cv;
  std::atomic_int i(-1);

  cpc<int, std::function<void(int)>> executor(2, [&](const int v) {
    std::this_thread::sleep_for(std::chrono::seconds(v));

    std::unique_lock<std::mutex> lk(mutex);
    i = v;
    lk.unlock();
    cv.notify_one();
  });

  executor.push(1);
  EXPECT_EQ(-1, i);

  executor.push(0);
  {
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk);
    EXPECT_EQ(0, i);
    cv.notify_one();
  }

  {
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk);
    EXPECT_EQ(1, i);
    cv.notify_one();
  }
}

}  // namespace
