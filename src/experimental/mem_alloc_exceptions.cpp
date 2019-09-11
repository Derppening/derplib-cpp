#include "derplib/experimental/mem_alloc.h"

namespace derplib {
namespace experimental {

mem_alloc::bad_alloc::bad_alloc() : _what(nullptr) {}
mem_alloc::bad_alloc::bad_alloc(const char* what) : std::bad_alloc(), _what(what) {}
mem_alloc::bad_alloc::bad_alloc(const std::string& what) : std::bad_alloc(), _what(what.c_str()) {}

const char* mem_alloc::bad_alloc::what() const noexcept {
  return _what != nullptr ? _what : std::bad_alloc::what();
}

}  // namespace experimental
}  // namespace derplib
