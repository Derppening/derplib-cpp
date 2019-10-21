#include "derplib/base/stopwatch.h"

#include <chrono>

namespace derplib {
inline namespace base {
void stopwatch::start() {
  _is_active_ = true;
  _start_ = std::chrono::steady_clock::now();
}

void stopwatch::stop() {
  _is_active_ = false;
  _end_ = std::chrono::steady_clock::now();
}
}  // namespace base
}  // namespace derplib
