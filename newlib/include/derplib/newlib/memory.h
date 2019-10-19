#pragma once

#include <memory>

#include "derplib/stdext/memory.h"

namespace derplib {
namespace newlib {
/**
 * \brief Exception-safe replacement for `std::unique_ptr::operator[]`
 *
 * \tparam T element type
 * \tparam Deleter deleter type
 * \param uptr `std::unique_ptr` to get element from
 * \param index index of the element
 * \return Pointer to the element, or `nullptr` if the smart pointer is not owning an object.
 */
template<typename T, typename Deleter>
T* element_at(const std::unique_ptr<T[], Deleter>& uptr, std::size_t index) noexcept;

/**
 * \brief Exception-safe replacement for `std::unique_ptr::operator->`.
 *
 * \tparam T unique pointer type
 * \tparam Deleter deleter type
 * \tparam R return type
 * \tparam Args arguments to the method
 * \param uptr `std::unique_ptr` to invoke function on
 * \param func pointer to the function to invoke
 * \param args arguments to the function
 * \return `std::unique_ptr<R>` storing the result, or an empty `std::unique_ptr` if `uptr` is `nullptr`.
 */
// TODO(Derppening): Do the std::enable_if<std::is_invocable>> thing
template<typename T, typename Deleter, typename R, typename... Args>
std::unique_ptr<R> invoke(const std::unique_ptr<T, Deleter>& uptr, R (T::*func)(Args...), Args&&... args) noexcept;

}  // namespace newlib
}  // namespace derplib

#include "memory.ipp"
