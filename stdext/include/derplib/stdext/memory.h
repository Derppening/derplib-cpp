#pragma once

#include <memory>
#include <type_traits>

#include "version.h"

namespace derplib {
namespace stdext {

/**
 * \brief Drop-in backport for `std::make_unique`.
 *
 * \tparam T type of object in `std::unique_ptr`
 * \tparam Args argument types
 * \param args list of arguments to be passed to the constructor of `T`
 * \return `std::unique_ptr<T>`.
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);

/**
 * \brief Drop-in backport for `std::make_unique`.
 *
 * \tparam T type of object in `std::unique_ptr`
 * \param size size of array
 * \return `std::unique_ptr<T>`
 */
template<typename T>
std::unique_ptr<T> make_unique(std::size_t size);

template<typename T, typename... Args>
typename std::enable_if<std::is_array<T>::value>::type make_unique(Args&&... args) = delete;

}  // namespace stdext
}  // namespace derplib

#include "memory.ipp"
