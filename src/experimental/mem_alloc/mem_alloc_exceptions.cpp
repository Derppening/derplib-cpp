#include "derplib/experimental/mem_alloc/bad_alloc.h"

namespace derplib {
namespace experimental {

bad_alloc::bad_alloc() : _what(nullptr) {}
bad_alloc::bad_alloc(const char* what) : std::bad_alloc(), _what(what) {}
bad_alloc::bad_alloc(const std::string& what) : std::bad_alloc(), _what(what.c_str()) {}

const char* bad_alloc::what() const noexcept {
  return _what != nullptr ? _what : std::bad_alloc::what();
}

}  // namespace experimental
}  // namespace derplib
