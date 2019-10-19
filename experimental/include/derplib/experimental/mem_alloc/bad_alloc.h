#pragma once

#include <stdexcept>

namespace derplib {
namespace experimental {

/**
   * \brief Thrown when an allocation cannot be made with the current heap.
   *
   * Similar to `std::bad_alloc`, but also provides an optional message on why the allocation failed.
   */
class bad_alloc : public std::bad_alloc {
 public:
  bad_alloc();
  bad_alloc(const char* what);
  bad_alloc(const std::string& what);

  bad_alloc(const bad_alloc& other) = default;

  const char* what() const noexcept override;

 private:
  const char* _what;
};

}  // namespace experimental
}  // namespace derplib
