#include "derplib/base/stopwatch.h"

#include <chrono>

namespace derplib {
void stopwatch::start() {
  _is_active = true;
  _start = std::chrono::steady_clock::now();
}

void stopwatch::stop() {
  _is_active = false;
  _end = std::chrono::steady_clock::now();
}
}  // namespace derplib
